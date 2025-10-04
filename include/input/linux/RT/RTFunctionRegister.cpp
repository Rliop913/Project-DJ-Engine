#include "Common_Features.hpp"
#include "RTSocket.hpp"
#include <iostream>
#include <unistd.h>
void
RTSocket::RegisterFunctions()
{
    functionRegistry["GET_DEV"] = [this](const data_body &data) {
        std::cout << "got get dev" << std::endl;
        int sendFlag = Common_Features::LPSend(
            importants.host_socket, "got get dev. end test transmission.");
        if (sendFlag < 0) {
            return sendFlag;
        }
        std::cout << "sent get dev echo" << std::endl;
        return 0;
    };
    functionRegistry["END_SOCKET"] = [this](const data_body &data) {
        std::cout << "got end socket" << std::endl;
        Common_Features::LPSend(importants.host_socket,
                                "got end signal. end socket.");
        std::cout << "sent end socket echo" << std::endl;
        return 1;
    };
}