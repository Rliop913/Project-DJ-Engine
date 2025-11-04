#include "Input_State.hpp"
#include "PDJE_Input.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <string>
#include <thread>
// #include "linux/linux_input.hpp"
#include <iostream>
// #include <unistd.h>
#include "MainProcess.hpp"
#include <filesystem>
#include <format>
namespace fs = std::filesystem;
int
main()
{
    // std::cout << GenExecuteShell("./PDJE_MODULE_INPUT_PROCESS", 84300)
    //           << std::endl;
    // auto mp = PDJE_IPC::MainProcess(54335);
    // std::cout << "opened connection" << std::endl;
    // if (mp.EndTransmission()) {
    //     std::cout << "Ended Transmission" << std::endl;
    // } else {
    //     std::cout << "Failed to End Transmission" << std::endl;
    // }

    // return 0;
    PDJE_Input pip;
    pip.Init();
    auto     devs = pip.GetDevs();
    DEV_LIST set_targets;
    for (auto i : devs) {
        std::cout << "name: " << i.Name << std::endl;
        switch (i.Type) {
        case PDJE_Dev_Type::MOUSE:
            std::cout << "type: mouse" << std::endl;
            break;
        case PDJE_Dev_Type::KEYBOARD:
            std::cout << "type: keyboard" << std::endl;
            set_targets.push_back(i);
            break;
        case PDJE_Dev_Type::HID:
            std::cout << "type: hid" << std::endl;
            break;
        case PDJE_Dev_Type::UNKNOWN:
            std::cout << "type: unknown" << std::endl;
            break;
        default:
            break;
        }

        std::cout << "dev path: " << i.device_specific_id << std::endl;
    }

    pip.Config(set_targets);
    // pip.NEXT();
    
    auto dline = pip.PullOutDataLine();
    pip.Run();//todo - impl process terminator
    // pip.NEXT();
    int         times = 100;
    std::thread watcher([&]() {
        while (true) {
            try{

                auto got = dline.input_arena->Get();
                for (uint64_t idx = 0; idx < got.second; ++idx)
                
                {
                        
                    std::cout << "time: " << got.first[idx].microSecond << std::endl;
                    std::cout << "id: " << got.first[idx].id << std::endl;
                    std::cout << "name: " << got.first[idx].name << std::endl;
                    
                    if (got.first[idx].type == PDJE_Dev_Type::KEYBOARD) {
                        
                        std::cout
                        << "keyNumber: "
                        << static_cast<int>(got.first[idx].event.keyboard.k)
                        << std::endl;
                        std::cout << "pressed"
                        << got.first[idx].event.keyboard.pressed
                        << std::endl;
                    }
                    
                    times--;
                    if (times < 0) {
                        return;
                    }
                    // }
                }
            } catch(const std::exception& e){
                std::cout << e.what() << std::endl;
            }
        }
    });

    watcher.join();
    pip.Kill();

    // OS_Input linux_oi;
    // linux_oi.SocketOpen("./PDJE_MODULE_INPUT_RTMAIN");
    // auto     devs = linux_oi.getDevices();
    // DEV_LIST toSet;
    // for (const auto &i : devs) {
    //     std::cout << i.Name << ", " << i.Type << std::endl;
    //     if (i.Type == "MOUSE") {
    //         toSet.push_back(i);
    //     }
    // }
    // std::cout << linux_oi.setDevices(toSet) << std::endl;
    // sleep(1);
    // linux_oi.EndSocketTransmission();
    // sleep(1);
    // std::cout << "end server" << std::endl;
    return 0;
}