#include "Input_State.hpp"
#include "PDJE_Input.hpp"
#include "linux/linux_input.hpp"
#include <iostream>
#include <unistd.h>
int
main()
{
    // auto pin = PDJE_Input();
    // auto dev = pin.GetDevs();
    // for(auto& i : dev){
    //     std::cout << i.Type << ", ";
    //     std::wcout << i.Name << std::endl;
    // }
    OS_Input linux_oi;
    linux_oi.SocketOpen("./PDJE_MODULE_INPUT_RTMAIN");
    auto     devs = linux_oi.getDevices();
    DEV_LIST toSet;
    for (const auto &i : devs) {
        std::cout << i.Name << ", " << i.Type << std::endl;
        if (i.Type == "MOUSE") {
            toSet.push_back(i);
        }
    }
    std::cout << linux_oi.setDevices(toSet) << std::endl;
    sleep(1);
    linux_oi.EndSocketTransmission();
    sleep(1);
    std::cout << "end server" << std::endl;
    return 0;
}