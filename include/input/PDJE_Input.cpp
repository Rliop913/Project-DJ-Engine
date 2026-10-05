#include "PDJE_Input.hpp"
#include "PDJE_Input_StateLogic.hpp"
#include "PDJE_LOG_SETTER.hpp"
PDJE_Input::PDJE_Input()
{
}

void
PDJE_Input::EnsureDefaultDevices()
{
    if (default_devs) {
        return;
    }
    try {
        default_devs.emplace();
        default_devs->SetPlatformContexts(
            platform_ctx0_, platform_ctx1_, use_internal_window_);
        default_devs->Ready();
    } catch (...) {
        default_devs.reset();
        throw;
    }
}

bool
PDJE_Input::Init(void *platform_ctx0,
                 void *platform_ctx1,
                 bool  use_internal_window)
{
    try {
        startlog();
        if (!PDJE_INPUT_STATE_LOGIC::CanInit(state)) {
            critlog(
                "pdje input module init failed. pdje input state is not dead. "
                "maybe input module is running or configuring.");
            return false;
        }
        platform_ctx0_       = platform_ctx0;
        platform_ctx1_       = platform_ctx1;
        use_internal_window_ = use_internal_window;
        midi_engine.emplace(64, midi_backend);
        state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
        return true;
    } catch (const std::exception &e) {
        critlog("failed to execute code. WHY: ");
        critlog(e.what());
        Kill();
        return false;
    }
}

bool
PDJE_Input::Config(std::vector<DeviceData>                  &devs,
                   const std::vector<libremidi::input_port> &midi_dev)
{
    return Config(devs, midi_dev, true);
}

bool
PDJE_Input::Config(std::vector<DeviceData>                  &devs,
                   const std::vector<libremidi::input_port> &midi_dev,
                   bool                                      midi_cc_lsb_on)
{
    try {
        if (!PDJE_INPUT_STATE_LOGIC::CanConfig(state)) {
            critlog(
                "pdje input module config failed. pdje input state is not on "
                "device config state. Init it first.");
            return false;
        }

        const bool              has_midi = !midi_dev.empty();
        std::vector<DeviceData> sanitized_devs =
            PDJE_INPUT_STATE_LOGIC::SanitizeConfigDevices(devs);
        const bool has_valid_input = !sanitized_devs.empty();
        bool       backend_ok      = false;

        if (has_valid_input) {
            EnsureDefaultDevices();
            backend_ok = default_devs->Config(sanitized_devs);
        }

        const auto decision = PDJE_INPUT_STATE_LOGIC::DecideConfigOutcome(
            has_valid_input, has_midi, backend_ok);

        if (!decision.success) {
            if (decision.backend_fail_path) {
                critlog("failed to configure devices.");
                Kill();
            }
            return false;
        }

        if (has_midi) {
            midi_engine->configed_devices = midi_dev;
        }
        FLAG_MIDI_ON    = has_midi;
        FLAG_INPUT_ON   = decision.flag_input_on;
        midi_cc_lsb_on_ = midi_cc_lsb_on;
        state           = decision.next_state;
        return true;
    } catch (const std::exception &e) {
        critlog("failed to config. WHY: ");
        critlog(e.what());
        Kill();
        return false;
    }
}

bool
PDJE_Input::Run()
{
    if (!PDJE_INPUT_STATE_LOGIC::CanRun(state)) {
        warnlog("pdje init module run failed. pdje input state is not on loop "
                "ready state. config it first.");
        return false;
    }

    try {
        if (FLAG_INPUT_ON) {
            default_devs->RunLoop();
        }
        if (FLAG_MIDI_ON) {
            midi_engine->Run(midi_cc_lsb_on_);
        }
        state = PDJE_INPUT_STATE::INPUT_LOOP_RUNNING;
        return true;
    } catch (const std::exception &e) {
        critlog("failed to run input devices. WHY: ");
        critlog(e.what());
        Kill();
        return false;
    }
}

bool
PDJE_Input::Kill()
{
    bool ok = true;
    try {
        const auto action = PDJE_INPUT_STATE_LOGIC::ClassifyKillAction(state);
        if (action == PDJE_INPUT_STATE_LOGIC::KillAction::BrokenState) {
            critlog("the pdje input module state is broken...why?");
            ok = false;
        }
        if (default_devs) {
            if (FLAG_INPUT_ON &&
                action == PDJE_INPUT_STATE_LOGIC::KillAction::TerminateLoop) {
                default_devs->TerminateLoop();
            } else {
                ok = default_devs->Kill() && ok;
            }
        }
    } catch (const std::exception &e) {
        critlog("failed to stop input devices. WHY: ");
        critlog(e.what());
        ok = false;
    }
    // Also release partial initialization, even if state is still DEAD.
    midi_engine.reset();

    default_devs.reset();
    FLAG_INPUT_ON        = false;
    FLAG_MIDI_ON         = false;
    platform_ctx0_       = nullptr;
    platform_ctx1_       = nullptr;
    use_internal_window_ = false;
    state                = PDJE_INPUT_STATE::DEAD;
    return ok;
}

std::vector<DeviceData>
PDJE_Input::GetDevs()
{
    if (state == PDJE_INPUT_STATE::DEAD) {
        return {};
    }
    EnsureDefaultDevices();
    return default_devs->GetDevices();
}

std::vector<libremidi::input_port>
PDJE_Input::GetMIDIDevs()
{
    return midi_engine ? midi_engine->GetDevices()
                       : std::vector<libremidi::input_port>{};
}

PDJE_INPUT_STATE
PDJE_Input::GetState()
{
    return state;
}

std::string
PDJE_Input::GetCurrentInputBackend() const
{
    if (!default_devs) {
        return "none";
    }
    return default_devs->GetCurrentBackendString();
}

PDJE_INPUT_DATA_LINE
PDJE_Input::PullOutDataLine()
{
    PDJE_INPUT_DATA_LINE line;
    if (FLAG_INPUT_ON) {
        line.input_arena = default_devs->GetInputBufferPTR();
    }
    if (FLAG_MIDI_ON) {
        line.midi_datas = &midi_engine->GetEventBuffer();
    }
    return line; // you should check nullptr before use.
}
