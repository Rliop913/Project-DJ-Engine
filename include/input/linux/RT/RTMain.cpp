#include "RTSocket.hpp"
#include <iostream>
int
main(int argc, char **argv)
{
    if (argc < 1) {
        return -1;
    }
    RTSocket rs = RTSocket();
    if (!rs.FixCPU()) {
        std::cout << rs.ErrMsg;
        return -2;
    }
    rs.MLock();

    rs.SocketOpen(argv[1]);

    rs.SocketClose();
    return 0;
}