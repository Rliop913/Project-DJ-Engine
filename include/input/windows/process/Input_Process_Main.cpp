#include "ChildProcess.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
    if(argc < 2){
        return -1;
    }
    try{

        PDJE_IPC::ChildProcess serv;
        auto port = std::stoi(argv[1]);
        std::cout << "open transmission on port " << argv[1] << std::endl;
        serv.RunServer(port);
        if(serv.KillCheck){
            std::cout << "Ended Transmission on server. killed on configure state." <<std::endl;
            return 0;
        }
        serv.LoopTrig();
    }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
        critlog(e.what());
    }

    std::cout << "Ended Transmission on server" <<std::endl;
    return 0;
}
