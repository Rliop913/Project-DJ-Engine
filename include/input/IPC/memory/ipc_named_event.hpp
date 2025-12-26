#pragma once
#include <filesystem>
namespace PDJE_IPC {
using MNAME = std::filesystem::path;
class EVENT {

  public:
    void *hdlp = nullptr;
    void
    Wait(const uint16_t waittime_ms = 100);
    void
    Wait_Infinite();
    void
    Wake();
    void
    HostInit(const MNAME &name);
    void
    ClientInit(const MNAME &name);
    EVENT() = default;
    ~EVENT();
};
}; // namespace PDJE_IPC