#include "TimeLineDiffMachine.hpp"

#include "PDJE_LOG_SETTER.hpp"

#include <algorithm>
#include <cctype>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

#include <git2/blob.h>
#include <git2/commit.h>
#include <git2/diff.h>
#include <git2/errors.h>
#include <git2/tree.h>

namespace PDJE_TIMELINE {
namespace {

using Core = TimeLineDiffMachineCore;

struct SemanticNoopBucketKey {
    Core::ParseMode       parse_mode  = Core::ParseMode::DirectObject;
    Core::RegionKindGuess region_kind = Core::RegionKindGuess::KvField;
    std::string           json_dump;

    bool
    operator<(const SemanticNoopBucketKey &rhs) const
    {
        if (parse_mode != rhs.parse_mode) {
            return static_cast<int>(parse_mode) <
                   static_cast<int>(rhs.parse_mode);
        }
        if (region_kind != rhs.region_kind) {
            return static_cast<int>(region_kind) <
                   static_cast<int>(rhs.region_kind);
        }
        return json_dump < rhs.json_dump;
    }
};

struct SemanticNoopBucketSides {
    std::vector<std::size_t> origin_indices;
    std::vector<std::size_t> compare_indices;
};

std::vector<Core::JsonizedRegion>
PruneSemanticNoopRegionPairs(std::vector<Core::JsonizedRegion> regions)
{
    if (regions.empty()) {
        return regions;
    }

    std::map<SemanticNoopBucketKey, SemanticNoopBucketSides> buckets;
    for (std::size_t i = 0; i < regions.size(); ++i) {
        SemanticNoopBucketKey key;
        key.parse_mode  = regions[i].parse_mode;
        key.region_kind = regions[i].region_kind;
        key.json_dump   = regions[i].json_value.dump();

        auto &bucket = buckets[key];
        if (regions[i].side == Core::DiffSide::Origin) {
            bucket.origin_indices.push_back(i);
        } else {
            bucket.compare_indices.push_back(i);
        }
    }

    std::vector<bool> cancelled(regions.size(), false);
    for (const auto &[_, bucket] : buckets) {
        const auto cancel_count =
            std::min(bucket.origin_indices.size(), bucket.compare_indices.size());
        for (std::size_t i = 0; i < cancel_count; ++i) {
            cancelled[bucket.origin_indices[i]]  = true;
            cancelled[bucket.compare_indices[i]] = true;
        }
    }

    std::vector<Core::JsonizedRegion> pruned;
    pruned.reserve(regions.size());
    for (std::size_t i = 0; i < regions.size(); ++i) {
        if (!cancelled[i]) {
            pruned.push_back(std::move(regions[i]));
        }
    }
    return pruned;
}

} // namespace

TimeLineDiffMachineCore::TimeLineDiffMachineCore(git_repository    *repo_,
                                                 const std::string &target_file_,
                                                 const OID         &origin_,
                                                 const OID         &compare_)
    : repo(repo_), target_file(target_file_), origin(origin_), compare(compare_)
{
}

std::optional<TimeLineSemanticDiffResult>
TimeLineDiffMachineCore::RunPipelineCore(TimeLineDiffKind kind, const TypeHooks &hooks)
{
    if (!repo) {
        critlog("timeline diff: repo is nullptr.");
        return std::nullopt;
    }
    if (!hooks.recovery_modes || !hooks.append_region) {
        critlog("timeline diff: invalid type hooks.");
        return std::nullopt;
    }

    auto changed = CollectChangedLines();
    if (!changed) {
        return std::nullopt;
    }

    TimeLineSemanticDiffResult empty;
    empty.kind    = kind;
    empty.origin  = origin;
    empty.compare = compare;
    if (changed->lines.empty()) {
        return empty;
    }

    auto origin_text  = LoadBlobTextFromCommit(origin);
    auto compare_text = LoadBlobTextFromCommit(compare);
    if (!origin_text || !compare_text) {
        return std::nullopt;
    }

    auto origin_blob  = MakeBlobText(std::move(*origin_text));
    auto compare_blob = MakeBlobText(std::move(*compare_text));

    auto recovered = RecoverFragmentsFromChangedLines(
        hooks, origin_blob, compare_blob, changed->lines, changed->hunks);
    if (recovered.empty()) {
        critlog("timeline diff: recovery stage produced no fragments.");
        critlog(origin);
        critlog(compare);
        return std::nullopt;
    }

    auto finalized = FinalizeRecoveredFragments(recovered);
    if (finalized.empty()) {
        critlog("timeline diff: finalize stage produced no regions.");
        return std::nullopt;
    }

    auto jsonized = JsonizeRegions(finalized);
    if (!jsonized) {
        return std::nullopt;
    }

    TimeLineSemanticDiffResult result;
    result.kind    = kind;
    result.origin  = origin;
    result.compare = compare;
    auto pruned_jsonized = PruneSemanticNoopRegionPairs(std::move(*jsonized));

    for (const auto &region : pruned_jsonized) {
        if (!hooks.append_region(region, result)) {
            critlog("timeline diff: struct parse stage failed.");
            critlog(std::to_string(region.start_line));
            critlog(std::to_string(region.end_line));
            return std::nullopt;
        }
    }
    return result;
}

bool
TimeLineDiffMachineCore::HasGitErr()
{
    return git_error_last() && git_error_last()->message;
}

void
TimeLineDiffMachineCore::LogGitErr(const char *prefix)
{
    critlog(prefix);
    if (HasGitErr()) {
        critlog(git_error_last()->message);
    }
}

std::string
TimeLineDiffMachineCore::TrimCopy(std::string_view sv)
{
    std::size_t start = 0;
    std::size_t end   = sv.size();
    while (start < end &&
           std::isspace(static_cast<unsigned char>(sv[start])) != 0) {
        ++start;
    }
    while (end > start &&
           std::isspace(static_cast<unsigned char>(sv[end - 1])) != 0) {
        --end;
    }
    return std::string(sv.substr(start, end - start));
}

std::string
TimeLineDiffMachineCore::TrimTrailingComma(std::string_view sv)
{
    std::string out = TrimCopy(sv);
    while (!out.empty() &&
           std::isspace(static_cast<unsigned char>(out.back())) != 0) {
        out.pop_back();
    }
    if (!out.empty() && out.back() == ',') {
        out.pop_back();
    }
    while (!out.empty() &&
           std::isspace(static_cast<unsigned char>(out.back())) != 0) {
        out.pop_back();
    }
    return out;
}

std::string
TimeLineDiffMachineCore::Preview(const std::string &s, std::size_t max_len)
{
    if (s.size() <= max_len) {
        return s;
    }
    return s.substr(0, max_len) + "...";
}

TimeLineDiffMachineCore::LineIndex
TimeLineDiffMachineCore::BuildLineIndex(const std::string &text)
{
    LineIndex idx;
    idx.line_starts.push_back(0);
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\n' && i + 1 < text.size()) {
            idx.line_starts.push_back(i + 1);
        }
    }
    if (text.empty()) {
        idx.line_starts = { 0 };
    }
    return idx;
}

bool
TimeLineDiffMachineCore::HasLine(const LineIndex &idx, int line)
{
    return line >= 1 && line <= idx.LineCount();
}

int
TimeLineDiffMachineCore::ClampLine(const LineIndex &idx, int line)
{
    if (idx.LineCount() <= 0) {
        return 1;
    }
    if (line < 1) {
        return 1;
    }
    if (line > idx.LineCount()) {
        return idx.LineCount();
    }
    return line;
}

std::size_t
TimeLineDiffMachineCore::LineStartOffset(const LineIndex &idx, int line)
{
    if (!HasLine(idx, line)) {
        return 0;
    }
    return idx.line_starts[static_cast<std::size_t>(line - 1)];
}

std::size_t
TimeLineDiffMachineCore::LineEndOffsetExclusive(const LineIndex   &idx,
                                                const std::string &text,
                                                int                line)
{
    if (!HasLine(idx, line)) {
        return text.size();
    }
    if (line == idx.LineCount()) {
        return text.size();
    }
    return idx.line_starts[static_cast<std::size_t>(line)];
}

int
TimeLineDiffMachineCore::OffsetToLine(const LineIndex &idx, std::size_t offset)
{
    auto it =
        std::upper_bound(idx.line_starts.begin(), idx.line_starts.end(), offset);
    if (it == idx.line_starts.begin()) {
        return 1;
    }
    return static_cast<int>(std::distance(idx.line_starts.begin(), it));
}

std::string_view
TimeLineDiffMachineCore::SliceByOffsets(const std::string &text,
                                        std::size_t        start,
                                        std::size_t        end_exclusive)
{
    if (start > end_exclusive || end_exclusive > text.size()) {
        return {};
    }
    return std::string_view(text.data() + start, end_exclusive - start);
}

std::optional<std::size_t>
TimeLineDiffMachineCore::FindEnclosingObjectStart(const std::string &text,
                                                  std::size_t        probe_offset)
{
    std::vector<std::pair<char, std::size_t>> stack;
    bool                                      in_string = false;
    bool                                      escape    = false;
    const auto end = std::min(probe_offset, text.size());
    for (std::size_t i = 0; i < end; ++i) {
        const char c = text[i];
        if (in_string) {
            if (escape) {
                escape = false;
                continue;
            }
            if (c == '\\') {
                escape = true;
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }
        if (c == '"') {
            in_string = true;
        } else if (c == '{' || c == '[') {
            stack.push_back({ c, i });
        } else if ((c == '}' || c == ']') && !stack.empty()) {
            stack.pop_back();
        }
    }
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        if (it->first == '{') {
            return it->second;
        }
    }
    return std::nullopt;
}

std::optional<std::size_t>
TimeLineDiffMachineCore::FindMatchingObjectEnd(const std::string &text,
                                               std::size_t        object_start)
{
    if (object_start >= text.size() || text[object_start] != '{') {
        return std::nullopt;
    }
    bool in_string = false;
    bool escape    = false;
    int  depth     = 0;
    for (std::size_t i = object_start; i < text.size(); ++i) {
        const char c = text[i];
        if (in_string) {
            if (escape) {
                escape = false;
                continue;
            }
            if (c == '\\') {
                escape = true;
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }
        if (c == '"') {
            in_string = true;
        } else if (c == '{') {
            ++depth;
        } else if (c == '}') {
            --depth;
            if (depth == 0) {
                std::size_t end_exclusive = i + 1;
                while (end_exclusive < text.size() &&
                       std::isspace(static_cast<unsigned char>(text[end_exclusive])) !=
                           0) {
                    ++end_exclusive;
                }
                if (end_exclusive < text.size() && text[end_exclusive] == ',') {
                    ++end_exclusive;
                }
                return end_exclusive;
            }
        }
    }
    return std::nullopt;
}

std::optional<TimeLineDiffMachineCore::CandidateOffsets>
TimeLineDiffMachineCore::RecoverObjectOffsetsAtLine(const BlobText &blob,
                                                    int             seed_line)
{
    if (!HasLine(blob.index, seed_line)) {
        return std::nullopt;
    }
    const auto probe_offset = LineStartOffset(blob.index, seed_line);
    const auto line_end     =
        LineEndOffsetExclusive(blob.index, blob.text, seed_line);

    std::optional<std::size_t> start_opt;
    std::size_t                cursor = probe_offset;
    while (cursor < line_end &&
           std::isspace(static_cast<unsigned char>(blob.text[cursor])) != 0) {
        ++cursor;
    }
    if (cursor < line_end && blob.text[cursor] == ',') {
        ++cursor;
        while (cursor < line_end &&
               std::isspace(static_cast<unsigned char>(blob.text[cursor])) != 0) {
            ++cursor;
        }
    }
    if (cursor < line_end && blob.text[cursor] == '{') {
        start_opt = cursor;
    } else {
        start_opt = FindEnclosingObjectStart(blob.text, probe_offset);
    }
    if (!start_opt) {
        return std::nullopt;
    }
    auto end_opt = FindMatchingObjectEnd(blob.text, *start_opt);
    if (!end_opt || *end_opt <= *start_opt) {
        return std::nullopt;
    }
    auto start_line = OffsetToLine(blob.index, *start_opt);
    auto end_line   = OffsetToLine(blob.index, *end_opt - 1);
    if ((end_line - start_line + 1) > kRecoveryMaxExpandLines) {
        return std::nullopt;
    }
    return CandidateOffsets{ *start_opt, *end_opt };
}

std::optional<TimeLineDiffMachineCore::CandidateOffsets>
TimeLineDiffMachineCore::RecoverTopLevelFieldOffsetsAtLine(const BlobText &blob,
                                                           int             seed_line)
{
    if (!HasLine(blob.index, seed_line)) {
        return std::nullopt;
    }
    auto line_start = LineStartOffset(blob.index, seed_line);
    auto line_end   = LineEndOffsetExclusive(blob.index, blob.text, seed_line);

    bool              in_string = false;
    bool              escape    = false;
    std::vector<char> stack;
    bool              in_field    = false;
    std::size_t       field_start = 0;

    for (std::size_t i = 0; i < blob.text.size(); ++i) {
        const char c = blob.text[i];
        if (in_string) {
            if (escape) {
                escape = false;
                continue;
            }
            if (c == '\\') {
                escape = true;
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }
        if (c == '"') {
            in_string = true;
            if (stack.size() == 1 && stack.back() == '{' && !in_field) {
                in_field    = true;
                field_start = i;
            }
            continue;
        }
        if (c == '{' || c == '[') {
            stack.push_back(c);
            continue;
        }
        if (c == '}' || c == ']') {
            if (stack.size() == 1 && stack.back() == '{' && in_field) {
                auto start_line_f = OffsetToLine(blob.index, field_start);
                auto end_line_f   = OffsetToLine(blob.index, i);
                if (seed_line >= start_line_f && seed_line <= end_line_f) {
                    return CandidateOffsets{ field_start, i };
                }
                in_field = false;
            }
            if (!stack.empty()) {
                stack.pop_back();
            }
            continue;
        }
        if (c == ',' && stack.size() == 1 && stack.back() == '{' && in_field) {
            auto start_line_f = OffsetToLine(blob.index, field_start);
            auto end_line_f   = OffsetToLine(blob.index, i);
            if (seed_line >= start_line_f && seed_line <= end_line_f) {
                return CandidateOffsets{ field_start, i + 1 };
            }
            in_field = false;
            continue;
        }
    }

    if (line_end > line_start) {
        return CandidateOffsets{ line_start, line_end };
    }
    return std::nullopt;
}

std::optional<TimeLineDiffMachineCore::RecoveredFragment>
TimeLineDiffMachineCore::TryRecoverFromSeedLine(const TypeHooks      &hooks,
                                                DiffSide              side,
                                                const BlobText       &blob,
                                                const ChangedLineRef &line_ref,
                                                int                   seed_line) const
{
    const auto modes = hooks.recovery_modes ? hooks.recovery_modes()
                                            : std::vector<RecoveryMode>{};
    for (auto mode : modes) {
        std::optional<CandidateOffsets> offsets;
        if (mode == RecoveryMode::Object) {
            offsets = RecoverObjectOffsetsAtLine(blob, seed_line);
        } else {
            offsets = RecoverTopLevelFieldOffsetsAtLine(blob, seed_line);
        }
        if (!offsets) {
            continue;
        }
        auto fragment_sv =
            SliceByOffsets(blob.text, offsets->start_offset, offsets->end_offset);
        if (fragment_sv.empty()) {
            continue;
        }

        bool valid = false;
        if (mode == RecoveryMode::Object) {
            if (!hooks.validate_object || !hooks.object_region_kind) {
                continue;
            }
            valid = hooks.validate_object(fragment_sv);
        } else {
            if (!hooks.validate_field || !hooks.field_region_kind) {
                continue;
            }
            valid = hooks.validate_field(fragment_sv);
        }
        if (!valid) {
            continue;
        }

        RecoveredFragment f;
        f.side          = side;
        f.hunk_id       = line_ref.hunk_id;
        f.start_line    = OffsetToLine(blob.index, offsets->start_offset);
        f.end_line      = OffsetToLine(blob.index, offsets->end_offset - 1);
        f.raw_fragment  = std::string(fragment_sv.begin(), fragment_sv.end());
        f.recovery_mode = mode;
        f.region_kind   = (mode == RecoveryMode::Object) ? hooks.object_region_kind()
                                                         : hooks.field_region_kind();
        f.source_lines.push_back(seed_line);
        return f;
    }
    return std::nullopt;
}

std::optional<std::string>
TimeLineDiffMachineCore::LoadBlobTextFromCommit(const OID &oid) const
{
    GIT_RAII::commit commit(oid, repo);
    if (!commit.p) {
        critlog("timeline diff: commit lookup failed.");
        critlog(oid);
        return std::nullopt;
    }

    GIT_RAII::tree tree;
    if (git_commit_tree(&tree.p, commit.p) != 0) {
        LogGitErr("timeline diff: commit tree lookup failed.");
        return std::nullopt;
    }

    GIT_RAII::tree_entry entry;
    if (git_tree_entry_bypath(&entry.p, tree.p, target_file.c_str()) != 0) {
        critlog("timeline diff: file not found in commit tree.");
        critlog(target_file);
        critlog(oid);
        if (HasGitErr()) {
            critlog(git_error_last()->message);
        }
        return std::nullopt;
    }

    GIT_RAII::blob blob;
    if (git_blob_lookup(&blob.p, repo, git_tree_entry_id(entry.p)) != 0) {
        LogGitErr("timeline diff: blob lookup failed.");
        return std::nullopt;
    }

    auto raw  = static_cast<const char *>(git_blob_rawcontent(blob.p));
    auto size = static_cast<std::size_t>(git_blob_rawsize(blob.p));
    return std::string(raw ? raw : "", size);
}

TimeLineDiffMachineCore::BlobText
TimeLineDiffMachineCore::MakeBlobText(std::string text)
{
    BlobText out;
    out.text  = std::move(text);
    out.index = BuildLineIndex(out.text);
    return out;
}

bool
TimeLineDiffMachineCore::LoadTreeForOid(const OID &oid, GIT_RAII::tree &tree_out) const
{
    GIT_RAII::commit commit(oid, repo);
    if (!commit.p) {
        critlog("timeline diff: commit lookup failed while loading tree.");
        critlog(oid);
        return false;
    }
    if (git_commit_tree(&tree_out.p, commit.p) != 0) {
        LogGitErr("timeline diff: commit tree lookup failed while loading tree.");
        return false;
    }
    return true;
}

int
TimeLineDiffMachineCore::DiffHunkCb(const git_diff_delta *,
                                    const git_diff_hunk *hunk,
                                    void                *payload_ptr)
{
    auto *payload = static_cast<DiffCollectPayload *>(payload_ptr);
    if (!payload || !hunk) {
        return 0;
    }
    HunkRange hr;
    hr.id        = payload->next_hunk_id++;
    hr.old_start = hunk->old_start;
    hr.old_lines = hunk->old_lines;
    hr.new_start = hunk->new_start;
    hr.new_lines = hunk->new_lines;
    payload->current_hunk = hr;
    payload->hunks[hr.id] = hr;
    return 0;
}

int
TimeLineDiffMachineCore::DiffLineCb(const git_diff_delta *,
                                    const git_diff_hunk *,
                                    const git_diff_line *line,
                                    void                *payload_ptr)
{
    auto *payload = static_cast<DiffCollectPayload *>(payload_ptr);
    if (!payload || !line || payload->current_hunk.id < 0) {
        return 0;
    }
    if (line->origin == GIT_DIFF_LINE_DELETION && line->old_lineno > 0) {
        payload->lines.push_back({ DiffSide::Origin,
                                   payload->current_hunk.id,
                                   static_cast<int>(line->old_lineno),
                                   line->origin,
                                   payload->current_hunk.old_start,
                                   payload->current_hunk.old_lines,
                                   payload->current_hunk.new_start,
                                   payload->current_hunk.new_lines });
    } else if (line->origin == GIT_DIFF_LINE_ADDITION && line->new_lineno > 0) {
        payload->lines.push_back({ DiffSide::Compare,
                                   payload->current_hunk.id,
                                   static_cast<int>(line->new_lineno),
                                   line->origin,
                                   payload->current_hunk.old_start,
                                   payload->current_hunk.old_lines,
                                   payload->current_hunk.new_start,
                                   payload->current_hunk.new_lines });
    }
    return 0;
}

std::optional<TimeLineDiffMachineCore::DiffCollectPayload>
TimeLineDiffMachineCore::CollectChangedLines() const
{
    GIT_RAII::tree old_tree;
    GIT_RAII::tree new_tree;
    if (!LoadTreeForOid(origin, old_tree) || !LoadTreeForOid(compare, new_tree)) {
        return std::nullopt;
    }

    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
    char            *paths[1] = { const_cast<char *>(target_file.c_str()) };
    opts.pathspec.strings     = paths;
    opts.pathspec.count       = 1;

    GIT_RAII::diff diff;
    if (git_diff_tree_to_tree(&diff.p, repo, old_tree.p, new_tree.p, &opts) != 0) {
        LogGitErr("timeline diff: git_diff_tree_to_tree failed.");
        return std::nullopt;
    }

    DiffCollectPayload payload;
    payload.current_hunk.id = -1;
    if (git_diff_foreach(diff.p, nullptr, nullptr, DiffHunkCb, DiffLineCb, &payload) !=
        0) {
        LogGitErr("timeline diff: git_diff_foreach failed.");
        return std::nullopt;
    }
    return payload;
}

std::vector<TimeLineDiffMachineCore::RecoveredFragment>
TimeLineDiffMachineCore::RecoverFragmentsFromChangedLines(
    const TypeHooks                         &hooks,
    const BlobText                          &origin_blob,
    const BlobText                          &compare_blob,
    const std::vector<ChangedLineRef>       &changed_lines,
    const std::unordered_map<int, HunkRange> &hunks) const
{
    std::vector<RecoveredFragment> fragments;
    std::set<SideHunkKey>          hunk_scan_keys;

    for (const auto &line_ref : changed_lines) {
        if (line_ref.hunk_id >= 0) {
            // Always scan both sides of touched hunks. This recovers semantic
            // rows hidden behind punctuation-only changes (for example `}` ->
            // `},`) so no-op pair pruning can cancel them.
            hunk_scan_keys.insert({ DiffSide::Origin, line_ref.hunk_id });
            hunk_scan_keys.insert({ DiffSide::Compare, line_ref.hunk_id });
        }
        const auto &blob =
            (line_ref.side == DiffSide::Origin) ? origin_blob : compare_blob;
        auto recovered =
            TryRecoverFromSeedLine(hooks, line_ref.side, blob, line_ref, line_ref.line_no);
        if (recovered) {
            fragments.push_back(std::move(*recovered));
        }
    }

    for (const auto &key : hunk_scan_keys) {
        auto hit = hunks.find(key.hunk_id);
        if (hit == hunks.end()) {
            continue;
        }
        const auto &hunk = hit->second;
        const auto &blob = (key.side == DiffSide::Origin) ? origin_blob : compare_blob;
        int start_line = key.side == DiffSide::Origin ? hunk.old_start : hunk.new_start;
        int line_count = key.side == DiffSide::Origin ? hunk.old_lines : hunk.new_lines;
        if (start_line <= 0) {
            start_line = 1;
        }
        if (line_count <= 0) {
            line_count = 1;
        }
        start_line = ClampLine(blob.index, start_line - kHunkRetryPadding);
        int end_line =
            ClampLine(blob.index, start_line + line_count + kHunkRetryPadding);

        ChangedLineRef retry_ref{};
        retry_ref.side           = key.side;
        retry_ref.hunk_id        = key.hunk_id;
        retry_ref.hunk_old_start = hunk.old_start;
        retry_ref.hunk_old_lines = hunk.old_lines;
        retry_ref.hunk_new_start = hunk.new_start;
        retry_ref.hunk_new_lines = hunk.new_lines;

        for (int line = start_line; line <= end_line; ++line) {
            retry_ref.line_no = line;
            auto recovered = TryRecoverFromSeedLine(hooks, key.side, blob, retry_ref, line);
            if (recovered) {
                fragments.push_back(std::move(*recovered));
            }
        }
    }
    return fragments;
}

std::vector<TimeLineDiffMachineCore::FinalizedRegion>
TimeLineDiffMachineCore::FinalizeRecoveredFragments(
    const std::vector<RecoveredFragment> &fragments)
{
    std::vector<FinalizedRegion> out;
    struct DedupKey {
        int         side;
        int         start_line;
        int         end_line;
        int         parse_mode;
        int         region_kind;
        std::string normalized_text;

        bool
        operator<(const DedupKey &rhs) const
        {
            if (side != rhs.side)
                return side < rhs.side;
            if (start_line != rhs.start_line)
                return start_line < rhs.start_line;
            if (end_line != rhs.end_line)
                return end_line < rhs.end_line;
            if (parse_mode != rhs.parse_mode)
                return parse_mode < rhs.parse_mode;
            if (region_kind != rhs.region_kind)
                return region_kind < rhs.region_kind;
            return normalized_text < rhs.normalized_text;
        }
    };

    std::map<DedupKey, std::size_t> dedup;
    for (const auto &f : fragments) {
        FinalizedRegion r;
        r.side       = f.side;
        r.start_line = f.start_line;
        r.end_line   = f.end_line;
        r.parse_mode = (f.recovery_mode == RecoveryMode::Object)
                           ? ParseMode::DirectObject
                           : ParseMode::WrappedField;
        r.region_kind     = f.region_kind;
        r.normalized_text = TrimTrailingComma(f.raw_fragment);
        if (r.normalized_text.empty()) {
            continue;
        }
        r.source_hunks.push_back(f.hunk_id);

        DedupKey key{ static_cast<int>(r.side),
                      r.start_line,
                      r.end_line,
                      static_cast<int>(r.parse_mode),
                      static_cast<int>(r.region_kind),
                      r.normalized_text };
        auto it = dedup.find(key);
        if (it == dedup.end()) {
            dedup.emplace(std::move(key), out.size());
            out.push_back(std::move(r));
        } else {
            auto &existing = out[it->second];
            if (std::find(existing.source_hunks.begin(),
                          existing.source_hunks.end(),
                          f.hunk_id) == existing.source_hunks.end()) {
                existing.source_hunks.push_back(f.hunk_id);
            }
        }
    }

    std::sort(out.begin(), out.end(), [](const FinalizedRegion &a, const FinalizedRegion &b) {
        if (a.side != b.side) {
            return static_cast<int>(a.side) < static_cast<int>(b.side);
        }
        int ah = a.source_hunks.empty()
                     ? std::numeric_limits<int>::max()
                     : *std::min_element(a.source_hunks.begin(), a.source_hunks.end());
        int bh = b.source_hunks.empty()
                     ? std::numeric_limits<int>::max()
                     : *std::min_element(b.source_hunks.begin(), b.source_hunks.end());
        if (ah != bh) {
            return ah < bh;
        }
        if (a.start_line != b.start_line) {
            return a.start_line < b.start_line;
        }
        return a.end_line < b.end_line;
    });

    return out;
}

std::optional<std::vector<TimeLineDiffMachineCore::JsonizedRegion>>
TimeLineDiffMachineCore::JsonizeRegions(const std::vector<FinalizedRegion> &regions)
{
    std::vector<JsonizedRegion> out;
    out.reserve(regions.size());
    for (const auto &r : regions) {
        try {
            JsonizedRegion jr;
            jr.side         = r.side;
            jr.start_line   = r.start_line;
            jr.end_line     = r.end_line;
            jr.parse_mode   = r.parse_mode;
            jr.region_kind  = r.region_kind;
            jr.source_hunks = r.source_hunks;

            if (r.parse_mode == ParseMode::DirectObject) {
                jr.json_value = nj::parse(r.normalized_text);
                if (!jr.json_value.is_object()) {
                    critlog("timeline diff: direct_object parse result is not object.");
                    critlog(Preview(r.normalized_text));
                    return std::nullopt;
                }
            } else {
                std::string wrapped = "{\n";
                wrapped += r.normalized_text;
                wrapped += "\n}";
                jr.json_value = nj::parse(wrapped);
                if (!jr.json_value.is_object() || jr.json_value.empty()) {
                    critlog("timeline diff: wrapped_field parse result invalid.");
                    critlog(Preview(r.normalized_text));
                    return std::nullopt;
                }
            }
            out.push_back(std::move(jr));
        } catch (const std::exception &e) {
            critlog("timeline diff: jsonize region failed.");
            critlog(e.what());
            critlog(Preview(r.normalized_text));
            return std::nullopt;
        }
    }
    return out;
}

} // namespace PDJE_TIMELINE
