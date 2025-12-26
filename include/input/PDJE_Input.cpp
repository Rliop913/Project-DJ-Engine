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

        Mproc.emplace();

        PDJE_IPC::Input_Transfer_Metadata cfg;
        PDJE_IPC::RANDOM_GEN              rg;
        cfg.max_length              = 2048;
        cfg.lenname                 = rg.Gen("PDJE_INPUT_LEN_");
        cfg.bodyname                = rg.Gen("PDJE_INPUT_BODY_");
        cfg.hmacname                = rg.Gen("PDJE_INPUT_HMAC_");
        cfg.data_request_event_name = rg.Gen("PDJE_INPUT_REQ_EVENT_");
        cfg.data_stored_event_name  = rg.Gen("PDJE_INPUT_STORED_EVENT_");
        input_buffer.emplace(cfg);
        Mproc->SendInputTransfer(input_buffer.value());
        Mproc->InitEvents();

        state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
        return true;
    } catch (const std::exception &e) {
        critlog("failed to execute code. WHY: ");
        critlog(e.what());
        return false;
    }
}

bool
PDJE_Input::Config(std::vector<DeviceData> &devs)
{
    try {
        std::vector<DeviceData> sanitized_devs;
        for (const auto &d : devs) {
            if (d.Name != "" && d.device_specific_id != "" &&
                d.Type != PDJE_Dev_Type::UNKNOWN) {
                sanitized_devs.push_back(d);
            }
        }
        if (state != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) {
            critlog(
                "pdje input module config failed. pdje input state is not on "
                "device config state. Init it first.");
            return false;
        }
        nlohmann::json nj;
        nj["body"] = nlohmann::json::array();
        for (const auto &dev : sanitized_devs) {
            std::unordered_map<std::string, std::string> kv;
            kv["id"]   = dev.device_specific_id;
            kv["name"] = dev.Name;
            switch (dev.Type) {
            case PDJE_Dev_Type::KEYBOARD:
                kv["type"] = "KEYBOARD";
                nj["body"].push_back(kv);
                break;
            case PDJE_Dev_Type::MOUSE:
                kv["type"] = "MOUSE";
                nj["body"].push_back(kv);
                break;
            case PDJE_Dev_Type::MIDI:
                kv["type"] = "MIDI";
                nj["body"].push_back(kv);
                break;
            case PDJE_Dev_Type::HID:
                kv["type"] = "HID";
                nj["body"].push_back(kv);
                break;
            default:
                break;
            }
        }

        Mproc->QueryConfig(nj.dump());
        state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
        return Mproc->EndTransmission();

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
    Mproc->events.input_loop_run_event.Wake();

    state = PDJE_INPUT_STATE::INPUT_LOOP_RUNNING;
    return true;
}

bool
PDJE_Input::Kill()
{
    switch (state) {
    case PDJE_INPUT_STATE::DEAD:
        return true;

    case PDJE_INPUT_STATE::DEVICE_CONFIG_STATE: {
        return Mproc->Kill();
    }
    case PDJE_INPUT_STATE::INPUT_LOOP_READY:
        Mproc->events.terminate_event.Wake();
        Mproc->events.input_loop_run_event.Wake();

        break;
    case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING: {
        Mproc->events.terminate_event.Wake();
        break;
    } break;
    default:
        critlog("the pdje input module state is broken...why?");
        return false;
    }
    state = PDJE_INPUT_STATE::DEAD;
    return true;
}

std::vector<DeviceData>
PDJE_Input::GetDevs()
{
    return Mproc->GetDevices();
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
    line.input_arena = &input_buffer.value();
    return line;
}