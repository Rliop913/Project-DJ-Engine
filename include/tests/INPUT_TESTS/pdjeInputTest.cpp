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
    linux_oi.getDevices();
    sleep(1);
    linux_oi.EndSocketTransmission();
    sleep(1);
    std::cout << "end server" << std::endl;
    return 0;
}