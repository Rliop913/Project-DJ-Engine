#include "TimeLineDiffMachine.hpp"

#include "PDJE_LOG_SETTER.hpp"

#include <cctype>
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
ValidateKvFieldFragment(std::string_view fragment)
{
    const auto norm = TrimTrailingCommaLocal(fragment);
    if (norm.empty()) {
        return false;
    }

    std::string wrapped = "{\n";
    wrapped += norm;
    wrapped += "\n}";
    auto parsed = TryParseJsonObjectLocal(wrapped);
    return parsed.has_value() && parsed->is_object() && !parsed->empty();
}

Core::RegionKindGuess
KvFieldRegionKind()
{
    return Core::RegionKindGuess::KvField;
}

std::vector<Core::RecoveryMode>
KvRecoveryModes()
{
    return { Core::RecoveryMode::Field };
}

bool
AppendKvRegion(const Core::JsonizedRegion &region, TimeLineSemanticDiffResult &result)
{
    if (region.region_kind != Core::RegionKindGuess::KvField) {
        critlog("timeline diff kv: unexpected region kind.");
        return false;
    }
    if (!region.json_value.is_object() || region.json_value.empty()) {
        critlog("timeline diff kv: region json is not object.");
        return false;
    }

    const bool removed = region.side == Core::DiffSide::Origin;
    for (auto &[k, v] : region.json_value.items()) {
        KEY_VALUE kv(k, v.dump());
        if (removed) {
            result.kvRemoved.push_back(std::move(kv));
        } else {
            result.kvAdded.push_back(std::move(kv));
        }
    }
    return true;
}

} // namespace

template <>
std::optional<TimeLineSemanticDiffResult>
TimeLineDiffMachine<KV_W>::Run()
{
    TypeHooks hooks{};
    hooks.validate_field    = &ValidateKvFieldFragment;
    hooks.field_region_kind = &KvFieldRegionKind;
    hooks.recovery_modes    = &KvRecoveryModes;
    hooks.append_region     = &AppendKvRegion;
    return RunPipelineCore(TimeLineDiffKind::KV, hooks);
}

} // namespace PDJE_TIMELINE
