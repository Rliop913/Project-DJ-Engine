#include "PSKPipe.hpp"

namespace PDJE_CRYPTO {

static std::string
GetTokenFromSTDPipe()
{

    return {};
}

void *
PSKPipe::Gen()
{
    try {
        return nullptr;

    } catch (const std::exception &e) {
        critlog("failed to generate token pipe. WHY: ");
        critlog(e.what());
        return nullptr;
    }
}
void
PSKPipe::Send(const std::string &msg)
{
    try {
        return;
    } catch (const std::exception &e) {
        critlog("failed to Send Token. Why: ");
        critlog(e.what());
    }
}
PSKPipe::~PSKPipe()
{
}
}; // namespace PDJE_CRYPTO