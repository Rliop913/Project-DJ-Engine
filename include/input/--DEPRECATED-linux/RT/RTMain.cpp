// #include "RTSocket.hpp"
#include "ChildProcess.hpp"
#include <cstdio>
#include <exception>
#include <iostream>
#include <memory>
int
main(int argc, char **argv)
{
    std::cout << "Hello from subProcess!!!" << std::endl;

    if (argc < 2) {
        return -1;
    }
    PDJE_IPC::TXRXListener serv;
    auto                   port = std::stoi(argv[1]);
    std::cout << "open transmission on port " << argv[1] << std::endl;
    serv.RunServer(port);
    std::cout << "Ended Transmission on server" << std::endl;
    return 0;
    // std::cout << "client on" << std::endl;
    // std::unique_ptr<RTSocket> rs;
    // RTEvent                   rtev;
    // try {
    //     rs = std::make_unique<RTSocket>(argv[1], &rtev);
    // } catch (const std::exception &e) {
    //     std::cerr << e.what() << std::endl;
    //     std::cout << "from RT" << std::endl;
    //     return -2;
    // }

    // std::cout << "in communication" << std::endl;
    // rs->Communication();
    // std::cout << "trig loop" << std::endl;
    // rtev.Trig();
    // std::cout << "end client" << std::endl;
    // run
    return 0;
}