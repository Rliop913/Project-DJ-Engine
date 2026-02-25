#pragma once

#include "../GitDataPack/GitRAII.hpp"
#include "../TimeLineDiffTypes.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace PDJE_TIMELINE {

class TimeLineDiffMachineCore {
  public:
    enum class DiffSide { Origin, Compare };
    enum class RecoveryMode { Object, Field };
    enum class ParseMode { DirectObject, WrappedField };
    enum class RegionKindGuess {
        MixRow,
        NoteRow,
        KvField,
        MusicBpmRow,
        MusicMetaField
    };

    struct HunkRange {
        int id        = -1;
        int old_start = 0;
        int old_lines = 0;
        int new_start = 0;
        int new_lines = 0;
    };

    struct ChangedLineRef {
        DiffSide side           = DiffSide::Origin;
        int      hunk_id        = -1;
        int      line_no        = 0;
        char     diff_origin    = '\0';
        int      hunk_old_start = 0;
        int      hunk_old_lines = 0;
        int      hunk_new_start = 0;
        int      hunk_new_lines = 0;
    };

    struct DiffCollectPayload {
        std::vector<ChangedLineRef>     lines;
        std::unordered_map<int, HunkRange> hunks;
        HunkRange                       current_hunk;
        int                             next_hunk_id = 0;
    };

    struct LineIndex {
        std::vector<std::size_t> line_starts;
        int
        LineCount() const
        {
            return static_cast<int>(line_starts.size());
        }
    };

    struct BlobText {
        std::string text;
        LineIndex   index;
    };

    struct CandidateOffsets {
        std::size_t start_offset = 0;
        std::size_t end_offset   = 0;
    };

    struct RecoveredFragment {
        DiffSide        side          = DiffSide::Origin;
        int             hunk_id       = -1;
        int             start_line    = 0;
        int             end_line      = 0;
        std::string     raw_fragment;
        RecoveryMode    recovery_mode = RecoveryMode::Object;
        RegionKindGuess region_kind   = RegionKindGuess::KvField;
        std::vector<int> source_lines;
    };

    struct FinalizedRegion {
        DiffSide        side        = DiffSide::Origin;
        int             start_line  = 0;
        int             end_line    = 0;
        ParseMode       parse_mode  = ParseMode::DirectObject;
        RegionKindGuess region_kind = RegionKindGuess::KvField;
        std::string     normalized_text;
        std::vector<int> source_hunks;
    };

    struct JsonizedRegion {
        DiffSide        side        = DiffSide::Origin;
        int             start_line  = 0;
        int             end_line    = 0;
        ParseMode       parse_mode  = ParseMode::DirectObject;
        RegionKindGuess region_kind = RegionKindGuess::KvField;
        std::vector<int> source_hunks;
        nj              json_value;
    };

  protected:
    struct TypeHooks {
        bool (*validate_object)(std::string_view)             = nullptr;
        bool (*validate_field)(std::string_view)              = nullptr;
        RegionKindGuess (*object_region_kind)()               = nullptr;
        RegionKindGuess (*field_region_kind)()                = nullptr;
        std::vector<RecoveryMode> (*recovery_modes)()         = nullptr;
        bool (*append_region)(const JsonizedRegion &,
                              TimeLineSemanticDiffResult &) = nullptr;
    };

    TimeLineDiffMachineCore(git_repository    *repo,
                            const std::string &target_file,
                            const OID         &origin,
                            const OID         &compare);

    std::optional<TimeLineSemanticDiffResult>
    RunPipelineCore(TimeLineDiffKind kind, const TypeHooks &hooks);

  private:
    struct SideHunkKey {
        DiffSide side   = DiffSide::Origin;
        int      hunk_id = -1;
        bool
        operator<(const SideHunkKey &rhs) const
        {
            if (side != rhs.side) {
                return static_cast<int>(side) < static_cast<int>(rhs.side);
            }
            return hunk_id < rhs.hunk_id;
        }
    };

    static constexpr int kRecoveryMaxExpandLines = 256;
    static constexpr int kHunkRetryPadding       = 1;

    git_repository *repo;
    std::string     target_file;
    OID             origin;
    OID             compare;

    static bool
    HasGitErr();
    static void
    LogGitErr(const char *prefix);

    static std::string
    TrimCopy(std::string_view sv);
    static std::string
    TrimTrailingComma(std::string_view sv);
    static std::string
    Preview(const std::string &s, std::size_t max_len = 160);

    static LineIndex
    BuildLineIndex(const std::string &text);
    static bool
    HasLine(const LineIndex &idx, int line);
    static int
    ClampLine(const LineIndex &idx, int line);
    static std::size_t
    LineStartOffset(const LineIndex &idx, int line);
    static std::size_t
    LineEndOffsetExclusive(const LineIndex &idx,
                           const std::string &text,
                           int                line);
    static int
    OffsetToLine(const LineIndex &idx, std::size_t offset);
    static std::string_view
    SliceByOffsets(const std::string &text,
                   std::size_t        start,
                   std::size_t        end_exclusive);

    static std::optional<std::size_t>
    FindEnclosingObjectStart(const std::string &text, std::size_t probe_offset);
    static std::optional<std::size_t>
    FindMatchingObjectEnd(const std::string &text, std::size_t object_start);
    static std::optional<CandidateOffsets>
    RecoverObjectOffsetsAtLine(const BlobText &blob, int seed_line);
    static std::optional<CandidateOffsets>
    RecoverTopLevelFieldOffsetsAtLine(const BlobText &blob, int seed_line);

    std::optional<RecoveredFragment>
    TryRecoverFromSeedLine(const TypeHooks     &hooks,
                           DiffSide             side,
                           const BlobText      &blob,
                           const ChangedLineRef &line_ref,
                           int                  seed_line) const;

    std::optional<std::string>
    LoadBlobTextFromCommit(const OID &oid) const;
    static BlobText
    MakeBlobText(std::string text);
    bool
    LoadTreeForOid(const OID &oid, GIT_RAII::tree &tree_out) const;

    static int
    DiffHunkCb(const git_diff_delta *, const git_diff_hunk *hunk, void *payload_ptr);
    static int
    DiffLineCb(const git_diff_delta *,
               const git_diff_hunk *,
               const git_diff_line *line,
               void                *payload_ptr);

    std::optional<DiffCollectPayload>
    CollectChangedLines() const;
    std::vector<RecoveredFragment>
    RecoverFragmentsFromChangedLines(const TypeHooks             &hooks,
                                     const BlobText              &origin_blob,
                                     const BlobText              &compare_blob,
                                     const std::vector<ChangedLineRef> &changed_lines,
                                     const std::unordered_map<int, HunkRange> &hunks) const;
    static std::vector<FinalizedRegion>
    FinalizeRecoveredFragments(const std::vector<RecoveredFragment> &fragments);
    static std::optional<std::vector<JsonizedRegion>>
    JsonizeRegions(const std::vector<FinalizedRegion> &regions);
};

template <typename CapnpType> class TimeLineDiffMachine : private TimeLineDiffMachineCore {
  public:
    TimeLineDiffMachine(git_repository    *repo,
                        const std::string &target_file,
                        const OID         &origin,
                        const OID         &compare)
        : TimeLineDiffMachineCore(repo, target_file, origin, compare)
    {
    }

    std::optional<TimeLineSemanticDiffResult>
    Run();

  protected:
    using TimeLineDiffMachineCore::DiffSide;
    using TimeLineDiffMachineCore::JsonizedRegion;
    using TimeLineDiffMachineCore::RecoveryMode;
    using TimeLineDiffMachineCore::RegionKindGuess;
    using TimeLineDiffMachineCore::RunPipelineCore;
    using TimeLineDiffMachineCore::TypeHooks;
};

template <>
std::optional<TimeLineSemanticDiffResult>
TimeLineDiffMachine<KV_W>::Run();
template <>
std::optional<TimeLineSemanticDiffResult>
TimeLineDiffMachine<MIX_W>::Run();
template <>
std::optional<TimeLineSemanticDiffResult>
TimeLineDiffMachine<NOTE_W>::Run();
template <>
std::optional<TimeLineSemanticDiffResult>
TimeLineDiffMachine<MUSIC_W>::Run();

}; // namespace PDJE_TIMELINE
