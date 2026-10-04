#pragma once

#include "Input_State.hpp"
#include "PDJE_Input_Device_Data.hpp"

#include <vector>

namespace PDJE_INPUT_STATE_LOGIC {

inline bool
IsValidConfigDevice(const DeviceData &d) noexcept
{
    return !d.Name.empty() && !d.device_specific_id.empty() &&
           d.Type != PDJE_Dev_Type::UNKNOWN;
}

inline std::vector<DeviceData>
SanitizeConfigDevices(const std::vector<DeviceData> &devs)
{
    std::vector<DeviceData> out;
    out.reserve(devs.size());
    for (const auto &d : devs) {
        if (IsValidConfigDevice(d)) {
            out.push_back(d);
        }
    }
    return out;
}

inline bool
CanInit(const PDJE_INPUT_STATE state) noexcept
{
    return state == PDJE_INPUT_STATE::DEAD;
}

inline bool
CanConfig(const PDJE_INPUT_STATE state) noexcept
{
    return state == PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
}

inline bool
CanRun(const PDJE_INPUT_STATE state) noexcept
{
    return state == PDJE_INPUT_STATE::INPUT_LOOP_READY;
}

struct ConfigDecision {
    bool             success           = false;
    bool             flag_input_on     = false;
    PDJE_INPUT_STATE next_state        = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
    bool             backend_fail_path = false;
};

inline ConfigDecision
DecideConfigOutcome(const bool has_valid_input,
                    const bool has_midi,
                    const bool backend_config_ok) noexcept
{
    ConfigDecision result;
    if (has_valid_input && !backend_config_ok) {
        result.backend_fail_path = true;
        result.next_state        = PDJE_INPUT_STATE::DEAD;
        return result;
    }
    result.success       = has_valid_input || has_midi;
    result.flag_input_on = has_valid_input && backend_config_ok;
    if (result.success) {
        result.next_state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
    }
    return result;
}

enum class KillAction { NoOp, BackendKill, TerminateLoop, BrokenState };

inline KillAction
ClassifyKillAction(const PDJE_INPUT_STATE state) noexcept
{
    switch (state) {
    case PDJE_INPUT_STATE::DEAD:
        return KillAction::NoOp;
    case PDJE_INPUT_STATE::DEVICE_CONFIG_STATE:
        return KillAction::BackendKill;
    case PDJE_INPUT_STATE::INPUT_LOOP_READY:
    case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING:
        return KillAction::TerminateLoop;
    default:
        return KillAction::BrokenState;
    }
}

} // namespace PDJE_INPUT_STATE_LOGIC
