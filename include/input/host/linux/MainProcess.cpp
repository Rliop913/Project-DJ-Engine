#include "MainProcess.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PSKPipe.hpp"
#include "RTEvent.hpp"
#include "ipc_named_event.hpp"
#include "ipc_util.hpp"
#include <libevdev/libevdev.h>
#include <linux/input.h>
#include <thread>

namespace PDJE_IPC {
using namespace MAINPROC;

// LINUX Doesn't use IPC features.

static inline void
SetEvdevGroupSettings()
{
    return; // set evdev group here.
    // todo - impl
}

TXRXTransport::~TXRXTransport()
{
}

TXRXTransport::TXRXTransport()
{
    SetEvdevGroupSettings();
}
bool
TXRXTransport::EndTransmission()
{
    return true;
}

bool
TXRXTransport::SendInputTransfer(PDJE_Input_Transfer &trsf)
{
    imp.rtev.set_Input_Transfer(&trsf);
    return true;
}
bool
TXRXTransport::CheckHealth()
{
    return true;
}

std::vector<DeviceData>
TXRXTransport::GetDevices()
{
    DEV_LIST lsdev;
    fs::path device_root("/dev/input/");

    for (const auto &dev : fs::directory_iterator(device_root)) {
        if (!dev.is_character_file()) {
            continue;
        }

        const std::string dev_path = dev.path().string();
        if (dev_path.find("event") == std::string::npos) {
            continue;
        }

        int FD = open(dev_path.c_str(), O_RDONLY | O_NONBLOCK);

        if (FD < 0) {
            continue;
        }
        libevdev  *info = nullptr;
        DeviceData dd;

        if (libevdev_new_from_fd(FD, &info) == 0) {
            const char *dev_name = libevdev_get_name(info);
            if (dev_name) {

                dd.Name               = std::string(dev_name);
                dd.device_specific_id = dev.path();
                if (!imp.stored_dev_path.contains(dd.Name)) {
                    imp.stored_dev_path[dd.Name].dev_path = dev.path();
                }

                if (libevdev_has_event_type(info, EV_KEY) &&
                    libevdev_has_event_code(info, EV_KEY, KEY_A) &&
                    libevdev_has_event_code(info, EV_KEY, KEY_SPACE) &&
                    libevdev_has_event_code(info, EV_KEY, KEY_ENTER)) {
                    dd.Type = PDJE_Dev_Type::KEYBOARD;
                    imp.stored_dev_path[dd.Name].dev_type =
                        PDJE_Dev_Type::KEYBOARD;
                } else if (libevdev_has_event_type(info, EV_REL) &&
                           libevdev_has_event_code(info, EV_REL, REL_X) &&
                           libevdev_has_event_code(info, EV_REL, REL_Y)) {
                    dd.Type = PDJE_Dev_Type::MOUSE;
                    imp.stored_dev_path[dd.Name].dev_type =
                        PDJE_Dev_Type::MOUSE;
                } else if (libevdev_has_event_type(info, EV_ABS) &&
                           (libevdev_has_event_code(
                                info, EV_KEY, BTN_GAMEPAD) ||
                            libevdev_has_event_code(
                                info, EV_KEY, BTN_JOYSTICK))) {
                    imp.stored_dev_path[dd.Name].dev_type =
                        PDJE_Dev_Type::UNKNOWN;
                    dd.Type = PDJE_Dev_Type::UNKNOWN;
                } else if (libevdev_has_event_type(info, EV_ABS) &&
                           libevdev_has_event_code(info, EV_KEY, BTN_TOUCH)) {
                    dd.Type = PDJE_Dev_Type::MOUSE;
                    imp.stored_dev_path[dd.Name].dev_type =
                        PDJE_Dev_Type::MOUSE;
                } else {
                    dd.Type = PDJE_Dev_Type::UNKNOWN;
                    imp.stored_dev_path[dd.Name].dev_type =
                        PDJE_Dev_Type::UNKNOWN;
                }
                lsdev.push_back(dd);
            }
            libevdev_free(info);
            close(FD);
        } else {
            close(FD);
            continue;
        }
    }
    return lsdev;
}

bool
TXRXTransport::QueryConfig(const std::string &dumped_json)
{
    DEV_LIST configed_devices;
    auto     nj = nlohmann::json::parse(dumped_json);
    for (const auto &i : nj["body"]) {
        DeviceData dd;
        dd.device_specific_id = i.at("id").get<std::string>();
        dd.Name               = i.at("name").get<std::string>();

        std::string tp = i.at("type").get<std::string>();
        if (tp == "KEYBOARD") {
            dd.Type = PDJE_Dev_Type::KEYBOARD;
        } else if (tp == "MOUSE") {
            dd.Type = PDJE_Dev_Type::MOUSE;
        } else {
            continue;
        }
        configed_devices.push_back(dd);
    }
    for (auto &i : configed_devices) {
        imp.rtev.Add(imp.stored_dev_path[i.Name].dev_path,
                     imp.stored_dev_path[i.Name].dev_type,
                     i.Name); // need to check stored dev path has
                              // device info first. todo - fix it.
    }

    std::thread nonrtThread([this]() {
        this->events.input_loop_run_event.Wait_Infinite();
        this->imp.rtev.Trig();
    });
    nonrtThread.detach(); // fix it later.

    return true;
}

void
TXRXTransport::InitEvents()
{

    auto namegen  = PDJE_IPC::RANDOM_GEN();
    auto loop_run = namegen.Gen("PDJE_IPC_EVENT_LOOP_RUN_");
    auto term     = namegen.Gen("PDJE_IPC_EVENT_TERMINATE_");

    events.input_loop_run_event.HostInit(loop_run);
    events.terminate_event.HostInit(term);
}
bool
TXRXTransport::Kill()
{
    return true;
}
}; // namespace PDJE_IPC