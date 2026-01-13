#include "PSKPipe.hpp"

// PSKPIPE is not used in linux.
namespace PDJE_CRYPTO {

static std::string
GetTokenFromSTDPipe()
{
    return {};
}

void *
PSKPipe::Gen()
{
    return nullptr;
}
void
PSKPipe::Send(const std::string &msg)
{
    return;
}
PSKPipe::~PSKPipe()
{
    return;
}
}; // namespace PDJE_CRYPTO