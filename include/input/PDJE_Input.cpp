#include "PDJE_Input.hpp"
#include "NameGen.hpp"
#include "PDJE_LOG_SETTER.hpp"
PDJE_Input::PDJE_Input()
{
    startlog();
}

bool
PDJE_Input::Init()
{
    try {
        if (state != PDJE_INPUT_STATE::DEAD) {
            critlog(
                "pdje input module init failed. pdje input state is not dead. "
                "maybe input module is running or configuring.");
            return false;
        }
        default_devs.emplace();
        default_devs->Ready();

        // #ifdef WIN32

        // PDJE_IPC::Input_Transfer_Metadata cfg;
        // PDJE_IPC::RANDOM_GEN              rg;
        // cfg.max_length              = 2048;
        // cfg.lenname                 = rg.Gen("PDJE_INPUT_LEN_");
        // cfg.bodyname                = rg.Gen("PDJE_INPUT_BODY_");
        // cfg.hmacname                = rg.Gen("PDJE_INPUT_HMAC_");
        // cfg.data_request_event_name = rg.Gen("PDJE_INPUT_REQ_EVENT_");
        // cfg.data_stored_event_name  = rg.Gen("PDJE_INPUT_STORED_EVENT_");
        // input_buffer.emplace(cfg);
        // default_devs->SendInputTransfer(input_buffer.value());
        // default_devs->InitEvents();
        // #endif
        midi_engine.emplace();
        state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
        return true;
    } catch (const std::exception &e) {
        critlog("failed to execute code. WHY: ");
        critlog(e.what());
        return false;
    }
}

bool
PDJE_Input::Config(std::vector<DeviceData>                  &devs,
                   const std::vector<libremidi::input_port> &midi_dev)
{
    try {
        if (!midi_dev.empty()) {
            midi_engine->configed_devices = midi_dev;
            FLAG_MIDI_ON                  = true;
        }

        if (state != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) {
            critlog(
                "pdje input module config failed. pdje input state is not on "
                "device config state. Init it first.");
            return false;
        }
        std::vector<DeviceData> sanitized_devs;
        for (const auto &d : devs) {
            if (d.Name != "" && d.device_specific_id != "" &&
                d.Type != PDJE_Dev_Type::UNKNOWN) {
                sanitized_devs.push_back(d);
            }
        }
        FLAG_INPUT_ON = (!sanitized_devs.empty());
        if (FLAG_INPUT_ON) {
            if (!default_devs->Config(sanitized_devs)) {
                critlog("failed to configure devices.");
                FLAG_INPUT_ON = false;
                return false;
            }
            state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
            return true;
        } else if (FLAG_MIDI_ON) { // fallback: only midi devices.
            state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
            return Kill();

        } else { // fallback: both invalid.
            return false;
        }
    } catch (const std::exception &e) {
        critlog("failed to config. WHY: ");
        critlog(e.what());
        return false;
    }
}

bool
PDJE_Input::Run()
{
    if (state != PDJE_INPUT_STATE::INPUT_LOOP_READY) {
        warnlog("pdje init module run failed. pdje input state is not on loop "
                "ready state. config it first.");
        return false;
    }

    default_devs->RunLoop();
    if (!FLAG_INPUT_ON) { // terminate if input flag is off.
        default_devs->TerminateLoop();
    }

    if (FLAG_MIDI_ON) { // run midi engine if midi flag is on.
        midi_engine->Run();
    }

    state = PDJE_INPUT_STATE::INPUT_LOOP_RUNNING;
    return true;
}

bool
PDJE_Input::Kill()
{
    bool ok = true;
    switch (state) {
    case PDJE_INPUT_STATE::DEAD:
        return true;

    case PDJE_INPUT_STATE::DEVICE_CONFIG_STATE: {
        if (default_devs) {
            // compatibility no-op for windows parity
            ok = default_devs->Kill();
        }
        break;
    }
    case PDJE_INPUT_STATE::INPUT_LOOP_READY:
        if (default_devs) {
            default_devs->TerminateLoop();
        }
        break;
    case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING: {
        if (default_devs) {
            default_devs->TerminateLoop();
        }
        break;
    }
    default:
        critlog("the pdje input module state is broken...why?");
        ok = false;
        break;
    }
    // reset datas.
    midi_engine.reset();

    default_devs.reset();
    FLAG_INPUT_ON = false;
    FLAG_MIDI_ON  = false;
    state         = PDJE_INPUT_STATE::DEAD;
    return ok;
}

std::vector<DeviceData>
PDJE_Input::GetDevs()
{

    return default_devs->GetDevices();
}

std::vector<libremidi::input_port>
PDJE_Input::GetMIDIDevs()
{
    return midi_engine->GetDevices();
}

PDJE_INPUT_STATE
PDJE_Input::GetState()
{
    return state;
}

PDJE_INPUT_DATA_LINE
PDJE_Input::PullOutDataLine()
{
    PDJE_INPUT_DATA_LINE line;
    if (FLAG_INPUT_ON) {
        line.input_arena = default_devs->GetInputBufferPTR();
    }
    if (FLAG_MIDI_ON) {
        line.midi_datas = &midi_engine->evlog;
    }
    return line; // you should check nullptr before use.
}
