#pragma once

#include "Input_State.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Crypto.hpp"
#include "PDJE_Highres_Clock.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "Secured_IPC_TX_RX.hpp"
#include "ipc_named_event.hpp"
#include "ipc_shared_memory.hpp"
#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
namespace PDJE_IPC {

using PDJE_DEV_PATH = std::string;
using PDJE_NAME     = std::string;

namespace SUBPROC {

class TXRXListener {
  private:
#ifdef WIN32
    DWORD ThreadID;
#elif defined(__linux__)

#endif
    std::optional<PDJE_CRYPTO::TX_RX>      txrx;
    std::unordered_map<PDJE_ID, PDJE_NAME> id_name;

    std::optional<PDJE_IPC::PDJE_Input_Transfer> input_buffer;

    EVENT input_loop_run_event;
    EVENT terminate_event;

    bool
    RecvIPCSharedMem(const std::string &mem_path,
                     const std::string &dataType,
                     const uint64_t     data_count);

    std::vector<DeviceData>                      configed_devices;
    std::unordered_map<PDJE_DEV_PATH, PDJE_NAME> unlisted_targets;

    std::string
    ListDev();

  public:
    bool KillCheck = false;
    TXRXListener(PDJE_CRYPTO::PSK      &psk,
                 const PDJE_IPC::MNAME &memFirst,
                 const PDJE_IPC::MNAME &firstLock,
                 const PDJE_IPC::MNAME &memSecond,
                 const PDJE_IPC::MNAME &secondLock);
    PDJE_HIGHRES_CLOCK::CLOCK timer;
    void
    BlockedListen()
    {
        txrx->BlockedListen();
    }
    void *
    Init();
    void
    LoopTrig();
    void
    Run();

    ~TXRXListener() = default;
};
}; // namespace SUBPROC
}; // namespace PDJE_IPC