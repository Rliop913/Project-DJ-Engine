#include "ChildProcess.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
    if(argc < 2){
        return -1;
    }

    PDJE_IPC::ChildProcess serv;
    auto port = std::stoi(argv[1]);
    std::cout << "open transmission on port " << argv[1] << std::endl;
    serv.RunServer(port);
    std::cout << "Ended Transmission on server" <<std::endl;
    return 0;
}
