#include "Input_State.hpp"
#include "PDJE_Input.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "DefaultDevs.hpp"
#include <algorithm>
#include <charconv>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

struct TestOptions {
    bool help                = false;
    bool use_internal_window = false;
    bool list_only           = false;
    bool probe_init_modes    = false;
    bool select_wayland_only = false;
    bool select_evdev_only   = false;
    int  max_events          = 100;
};

bool
StartsWith(const std::string &s, const std::string &prefix)
{
    return s.size() >= prefix.size() &&
           std::equal(prefix.begin(), prefix.end(), s.begin());
}

bool
IsWaylandSynthetic(const DeviceData &d)
{
    return StartsWith(d.device_specific_id, "wayland://");
}

const char *
DevTypeToString(PDJE_Dev_Type t)
{
    switch (t) {
    case PDJE_Dev_Type::MOUSE:
        return "mouse";
    case PDJE_Dev_Type::KEYBOARD:
        return "keyboard";
    case PDJE_Dev_Type::UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

void
PrintUsage(const char *argv0)
{
    std::cout
        << "Usage: " << argv0
        << " [--internal-window] [--list-only] [--probe-init-modes]\n"
        << "       [--wayland-only|--evdev-only] [--max-events N]\n\n"
        << "Examples:\n"
        << "  " << argv0 << " --probe-init-modes\n"
        << "  " << argv0 << " --internal-window --wayland-only --list-only\n"
        << "  " << argv0 << " --internal-window --wayland-only --max-events 20\n";
}

bool
ParseInt(const std::string &s, int &out)
{
    auto *begin = s.data();
    auto *end   = s.data() + s.size();
    int   v     = 0;
    auto  res   = std::from_chars(begin, end, v);
    if (res.ec != std::errc() || res.ptr != end) {
        return false;
    }
    out = v;
    return true;
}

bool
ParseArgs(int argc, char **argv, TestOptions &opt)
{
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            opt.help = true;
            continue;
        }
        if (arg == "--internal-window") {
            opt.use_internal_window = true;
            continue;
        }
        if (arg == "--list-only") {
            opt.list_only = true;
            continue;
        }
        if (arg == "--probe-init-modes") {
            opt.probe_init_modes = true;
            continue;
        }
        if (arg == "--wayland-only") {
            opt.select_wayland_only = true;
            continue;
        }
        if (arg == "--evdev-only") {
            opt.select_evdev_only = true;
            continue;
        }
        if (arg == "--max-events") {
            if (i + 1 >= argc) {
                std::cerr << "--max-events requires a value\n";
                return false;
            }
            ++i;
            if (!ParseInt(argv[i], opt.max_events) || opt.max_events < 0) {
                std::cerr << "invalid --max-events value: " << argv[i] << "\n";
                return false;
            }
            continue;
        }

        std::cerr << "unknown option: " << arg << "\n";
        PrintUsage(argv[0]);
        return false;
    }

    if (opt.select_wayland_only && opt.select_evdev_only) {
        std::cerr << "--wayland-only and --evdev-only cannot be used together\n";
        return false;
    }

    return true;
}

void
PrintBackendState(const std::string &label, PDJE_Input &pip)
{
    std::cout << "[backend] " << label << ": " << pip.GetCurrentInputBackend()
              << std::endl;
}

void
PrintDevices(const std::vector<DeviceData> &devs)
{
    std::cout << "[devices] count=" << devs.size() << std::endl;
    for (const auto &d : devs) {
        std::cout << "name: " << d.Name << std::endl;
        std::cout << "type: " << DevTypeToString(d.Type) << std::endl;
        std::cout << "dev path: " << d.device_specific_id << std::endl;
        std::cout << "backend-candidate: "
                  << (IsWaylandSynthetic(d) ? "wayland-synthetic" : "evdev")
                  << std::endl;
    }
}

DEV_LIST
BuildTargets(const std::vector<DeviceData> &devs, const TestOptions &opt)
{
    DEV_LIST set_targets;
    for (const auto &d : devs) {
        if (d.Type != PDJE_Dev_Type::MOUSE && d.Type != PDJE_Dev_Type::KEYBOARD) {
            continue;
        }
        const bool is_wayland = IsWaylandSynthetic(d);
        if (opt.select_wayland_only && !is_wayland) {
            continue;
        }
        if (opt.select_evdev_only && is_wayland) {
            continue;
        }
        set_targets.push_back(d);
    }
    return set_targets;
}

int
RunInitProbeMode()
{
    for (bool use_internal_window : { false, true }) {
        std::cout << "\n=== probe: Init(nullptr, nullptr, "
                  << (use_internal_window ? "true" : "false") << ") ===\n";

        PDJE_Input pip;
        PrintBackendState("before init", pip);
        const bool init_ok = pip.Init(nullptr, nullptr, use_internal_window);
        std::cout << "Init result: " << (init_ok ? "true" : "false")
                  << std::endl;
        PrintBackendState("after init", pip);

        if (init_ok) {
            auto devs = pip.GetDevs();
            PrintDevices(devs);
            int wayland_count = 0;
            for (const auto &d : devs) {
                if (IsWaylandSynthetic(d)) {
                    ++wayland_count;
                }
            }
            std::cout << "Synthetic wayland devices: " << wayland_count
                      << std::endl;
        }

        const bool kill_ok = pip.Kill();
        std::cout << "Kill result: " << (kill_ok ? "true" : "false")
                  << std::endl;
        PrintBackendState("after kill", pip);
    }
    return 0;
}

void
PrintTargetSummary(const DEV_LIST &targets)
{
    std::cout << "[config] selected targets: " << targets.size() << std::endl;
    for (const auto &d : targets) {
        std::cout << "  - " << d.Name << " (" << DevTypeToString(d.Type)
                  << ", " << d.device_specific_id << ")\n";
    }
}

} // namespace

int
main(int argc, char **argv)
{
    TestOptions opt;
    if (!ParseArgs(argc, argv, opt)) {
        return 1;
    }
    if (opt.help) {
        PrintUsage(argv[0]);
        return 0;
    }

    if (opt.probe_init_modes) {
        return RunInitProbeMode();
    }

    PDJE_Input pip;
    PrintBackendState("before init", pip);
    if (!pip.Init(nullptr, nullptr, opt.use_internal_window)) {
        std::cerr << "Init failed\n";
        return 2;
    }
    PrintBackendState("after init", pip);

    auto devs = pip.GetDevs();
    PrintDevices(devs);

    auto set_targets = BuildTargets(devs, opt);
    PrintTargetSummary(set_targets);

    if (opt.list_only) {
        const bool kill_ok = pip.Kill();
        std::cout << "Kill result: " << (kill_ok ? "true" : "false")
                  << std::endl;
        PrintBackendState("after kill", pip);
        return 0;
    }

    if (set_targets.empty()) {
        std::cerr << "No selectable input devices found for the requested filter\n";
        pip.Kill();
        return 3;
    }

    const bool cfg_ok =
        pip.Config(set_targets, std::vector<libremidi::input_port>());
    std::cout << "Config result: " << (cfg_ok ? "true" : "false") << std::endl;
    PrintBackendState("after config", pip);
    if (!cfg_ok) {
        pip.Kill();
        return 4;
    }

    auto dline = pip.PullOutDataLine();
    const bool run_ok = pip.Run();
    std::cout << "Run result: " << (run_ok ? "true" : "false") << std::endl;
    PrintBackendState("after run", pip);
    if (!run_ok) {
        pip.Kill();
        return 5;
    }
    if (!dline.input_arena) {
        std::cerr << "Input arena is null\n";
        pip.Kill();
        return 6;
    }

    int times = opt.max_events;
    std::thread watcher([&]() {
        while (times > 0) {
            try {
                dline.input_arena->Receive();
                const auto got = dline.input_arena->datas;
                for (const auto &idx : got) {
                    std::cout << "time: " << idx.microSecond << std::endl;
                    std::cout << "id: " << idx.id << std::endl;
                    std::cout << "name: " << idx.name << std::endl;

                    if (idx.type == PDJE_Dev_Type::KEYBOARD) {
                        std::cout << "keyNumber: "
                                  << static_cast<int>(idx.event.keyboard.k)
                                  << std::endl;
                        std::cout << "pressed " << idx.event.keyboard.pressed
                                  << std::endl;
                    } else if (idx.type == PDJE_Dev_Type::MOUSE) {
                        std::cout << "axisType: "
                                  << static_cast<int>(idx.event.mouse.axis_type)
                                  << std::endl;
                        std::cout << "value: " << idx.event.mouse.x << ", "
                                  << idx.event.mouse.y << std::endl;
                    }

                    --times;
                    if (times <= 0) {
                        return;
                    }
                }
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
    });

    watcher.join();
    const bool kill_ok = pip.Kill();
    std::cout << "Kill result: " << (kill_ok ? "true" : "false") << std::endl;
    PrintBackendState("after kill", pip);
    return 0;
}
