#include "DefaultDevs.hpp"
#include <exception>
#include <fcntl.h>
#include <unistd.h>
namespace PDJE_DEFAULT_DEVICES {

DefaultDevs::DefaultDevs() : input_buffer(1024)
{
}

void
DefaultDevs::Ready()
{
    if (!IC) {
        IC.emplace();
        IC->set_Input_Transfer(&input_buffer);
    }
}

bool
DefaultDevs::Config(const std::vector<DeviceData> &devs)
{
    try {
        if (!IC) {
            Ready();
        }
        std::size_t added_count = 0;
        for (const auto &i : devs) {
            if (i.Name.empty()) {
                continue;
            }

            auto searched = stored_dev.find(i.Name);
            if (searched != stored_dev.end()) {
                if (IC->Add(searched->second.dev_path,
                            searched->second.dev_type,
                            i.Name)) {
                    ++added_count;
                } else {
                    warnlog("failed to add input device on linux:");
                    warnlog(i.Name);
                }
            }
        }
        return added_count > 0;
    } catch (const std::exception &e) {
        critlog("failed on Device Configure on linux. What:");
        critlog(e.what());
        return false;
    }
}
DefaultDevs::~DefaultDevs()
{
}

std::vector<DeviceData>
DefaultDevs::GetDevices()
{
    DEV_LIST lsdev;
    fs::path device_root("/dev/input/");
    stored_dev.clear();

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
                dd.device_specific_id = dev.path().string();
                stored_dev[dd.Name].dev_path = dev.path();

                if (libevdev_has_event_type(info, EV_KEY) &&
                    libevdev_has_event_code(info, EV_KEY, KEY_A) &&
                    libevdev_has_event_code(info, EV_KEY, KEY_SPACE) &&
                    libevdev_has_event_code(info, EV_KEY, KEY_ENTER)) {
                    dd.Type                      = PDJE_Dev_Type::KEYBOARD;
                    stored_dev[dd.Name].dev_type = PDJE_Dev_Type::KEYBOARD;

                } else if (libevdev_has_event_type(info, EV_REL) &&
                           libevdev_has_event_code(info, EV_REL, REL_X) &&
                           libevdev_has_event_code(info, EV_REL, REL_Y)) {
                    dd.Type                      = PDJE_Dev_Type::MOUSE;
                    stored_dev[dd.Name].dev_type = PDJE_Dev_Type::MOUSE;

                } else if (libevdev_has_event_type(info, EV_ABS) &&
                           (libevdev_has_event_code(
                                info, EV_KEY, BTN_GAMEPAD) ||
                            libevdev_has_event_code(
                                info, EV_KEY, BTN_JOYSTICK))) {
                    stored_dev[dd.Name].dev_type = PDJE_Dev_Type::UNKNOWN;
                    dd.Type                      = PDJE_Dev_Type::UNKNOWN;

                } else if (libevdev_has_event_type(info, EV_ABS) &&
                           libevdev_has_event_code(info, EV_KEY, BTN_TOUCH)) {
                    dd.Type                      = PDJE_Dev_Type::MOUSE;
                    stored_dev[dd.Name].dev_type = PDJE_Dev_Type::MOUSE;

                } else {
                    dd.Type                      = PDJE_Dev_Type::UNKNOWN;
                    stored_dev[dd.Name].dev_type = PDJE_Dev_Type::UNKNOWN;
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

}; // namespace PDJE_DEFAULT_DEVICES
