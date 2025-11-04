#include "ChildProcess.hpp"

int main(int argc, char const *argv[])
{
    if(argc < 2){
        return -1;
    }
    try{

        PDJE_IPC::ChildProcess serv;
        auto port = std::stoi(argv[1]);
        serv.RunServer(port);
        if(serv.KillCheck){
            return 0;
        }
        serv.LoopTrig();
    }
    catch (const std::exception& e){
        critlog(e.what());
    }
    return 0;
}
