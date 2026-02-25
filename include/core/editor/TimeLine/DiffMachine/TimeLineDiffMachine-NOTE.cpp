#include "TimeLineDiffMachine.hpp"

#include "PDJE_LOG_SETTER.hpp"

#include <cctype>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace PDJE_TIMELINE {
namespace {

using Core = TimeLineDiffMachineCore;

std::string
TrimCopyLocal(std::string_view sv)
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
TrimTrailingCommaLocal(std::string_view sv)
{
    std::string out = TrimCopyLocal(sv);
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

std::optional<nj>
TryParseJsonObjectLocal(std::string_view sv)
{
    try {
        auto parsed = nj::parse(sv.begin(), sv.end());
        if (!parsed.is_object()) {
            return std::nullopt;
        }
        return parsed;
    } catch (...) {
        return std::nullopt;
    }
}

bool
JsonHasKeysLocal(const nj &obj, const std::initializer_list<const char *> &keys)
{
    if (!obj.is_object()) {
        return false;
    }
    for (const auto *k : keys) {
        if (!obj.contains(k)) {
            return false;
        }
    }
    return true;
}

bool
LooksLikeNoteRowJson(const nj &obj)
{
    return JsonHasKeysLocal(obj,
                            { PDJE_JSON_NOTE_TYPE, PDJE_JSON_NOTE_DETAIL,
                              PDJE_JSON_FIRST, PDJE_JSON_SECOND, PDJE_JSON_THIRD,
                              PDJE_JSON_BEAT, PDJE_JSON_SUBBEAT, PDJE_JSON_SEPARATE,
                              PDJE_JSON_EBEAT, PDJE_JSON_ESUBBEAT,
                              PDJE_JSON_ESEPARATE, PDJE_JSON_RAILID });
}

bool
ValidateNoteObjectFragment(std::string_view fragment)
{
    auto obj = TryParseJsonObjectLocal(TrimTrailingCommaLocal(fragment));
    return obj.has_value() && LooksLikeNoteRowJson(*obj);
}

Core::RegionKindGuess
NoteObjectRegionKind()
{
    return Core::RegionKindGuess::NoteRow;
}

std::vector<Core::RecoveryMode>
NoteRecoveryModes()
{
    return { Core::RecoveryMode::Object };
}

bool
JsonToNoteArgsLocal(const nj &obj, NoteArgs &out)
{
    try {
        out.Note_Type   = obj.at(PDJE_JSON_NOTE_TYPE).get<SANITIZED_ORNOT>();
        out.Note_Detail = obj.at(PDJE_JSON_NOTE_DETAIL).get<uint16_t>();
        out.first       = obj.at(PDJE_JSON_FIRST).get<SANITIZED_ORNOT>();
        out.second      = obj.at(PDJE_JSON_SECOND).get<SANITIZED_ORNOT>();
        out.third       = obj.at(PDJE_JSON_THIRD).get<SANITIZED_ORNOT>();
        out.beat        = obj.at(PDJE_JSON_BEAT).get<uint64_t>();
        out.subBeat     = obj.at(PDJE_JSON_SUBBEAT).get<uint64_t>();
        out.separate    = obj.at(PDJE_JSON_SEPARATE).get<uint64_t>();
        out.Ebeat       = obj.at(PDJE_JSON_EBEAT).get<uint64_t>();
        out.EsubBeat    = obj.at(PDJE_JSON_ESUBBEAT).get<uint64_t>();
        out.Eseparate   = obj.at(PDJE_JSON_ESEPARATE).get<uint64_t>();
        out.railID      = obj.at(PDJE_JSON_RAILID).get<uint64_t>();
        return true;
    } catch (const std::exception &e) {
        critlog("timeline diff note: NoteArgs parse failed.");
        critlog(e.what());
        return false;
    }
}

bool
AppendNoteRegion(const Core::JsonizedRegion &region, TimeLineSemanticDiffResult &result)
{
    if (region.region_kind != Core::RegionKindGuess::NoteRow) {
        critlog("timeline diff note: unexpected region kind.");
        return false;
    }
    if (!region.json_value.is_object()) {
        critlog("timeline diff note: region json is not object.");
        return false;
    }

    NoteArgs args;
    if (!JsonToNoteArgsLocal(region.json_value, args)) {
        return false;
    }

    if (region.side == Core::DiffSide::Origin) {
        result.noteRemoved.push_back(args);
    } else {
        result.noteAdded.push_back(args);
    }
    return true;
}

} // namespace

template <>
std::optional<TimeLineSemanticDiffResult>
TimeLineDiffMachine<NOTE_W>::Run()
{
    TypeHooks hooks{};
    hooks.validate_object    = &ValidateNoteObjectFragment;
    hooks.object_region_kind = &NoteObjectRegionKind;
    hooks.recovery_modes     = &NoteRecoveryModes;
    hooks.append_region      = &AppendNoteRegion;
    return RunPipelineCore(TimeLineDiffKind::NOTE, hooks);
}

} // namespace PDJE_TIMELINE
