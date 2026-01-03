#include "PDJE_Crypto.hpp"
#include "PSKPipe.hpp"
// #include "SubProcess.hpp"
#include <sstream>
int
main()
{
    try {
        startlog();

        // TODO - TEST RTKIT

        // auto tokenstr = PDJE_CRYPTO::PSKPipe::GetTokenFromSTDPipe();
        // std::istringstream spstrm(tokenstr);
        // std::string        pskhex;
        // std::string        mfirst;
        // std::string        lfirst;
        // std::string        msecond;
        // std::string        lsecond;
        // spstrm >> pskhex >> mfirst >> lfirst >> msecond >> lsecond;
        // auto psk = PDJE_CRYPTO::PSK();
        // psk.Decode(pskhex);

        // PDJE_IPC::SUBPROC::TXRXListener serv(
        //     psk, mfirst, lfirst, msecond, lsecond);
        // serv.BlockedListen();
        // if (serv.KillCheck) {
        //     return 0;
        // }
        // serv.LoopTrig();
    } catch (const std::exception &e) {
        critlog(e.what());
    }
    return 0;
}
