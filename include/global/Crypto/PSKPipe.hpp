#pragma once
#include "PDJE_LOG_SETTER.hpp"
#include <string>
namespace PDJE_CRYPTO {

class PSKPipe {
  private:
    void *writeHandle = nullptr;

  public:
    static std::string
    GetTokenFromSTDPipe();
    void *
    Gen();
    void
    Send(const std::string &msg);

    PSKPipe()
    {
        startlog();
    }
    ~PSKPipe();
};
}; // namespace PDJE_CRYPTO