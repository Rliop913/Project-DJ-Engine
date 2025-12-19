#include "PDJE_Crypto.hpp"
#include "PSKPipe.hpp"
#include "SubProcess.hpp"
#include <sstream>
int
main()
{
    try {
        startlog();
        auto tokenstr = PDJE_CRYPTO::PSKPipe::GetTokenFromSTDPipe();
        std::istringstream spstrm(tokenstr);
        std::string        pskhex;
        std::string        portstr;
        spstrm >> pskhex;
        spstrm >> portstr;
        auto psk = PDJE_CRYPTO::PSK();
        psk.Decode(pskhex);

        PDJE_IPC::SubProc serv(psk);
        int               port = std::stoi(portstr);
        serv.RunServer(port);
        if (serv.KillCheck) {
            return 0;
        }
        serv.LoopTrig();
    } catch (const std::exception &e) {
        critlog(e.what());
    }
    return 0;
}
