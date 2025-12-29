#pragma once
#include "MainProcess.hpp"
namespace PDJE_IPC {
using namespace MAINPROC;
bool
TXRXTransport::SendIPCSharedMemory(const uint64_t     mem_length,
                                   const std::string &mem_path,
                                   const std::string &dataType)
{
    nj j;
    j["PATH"]     = mem_path;
    j["DATATYPE"] = dataType;
    j["COUNT"]    = mem_length;
    try {
        TXRX_RESPONSE.SEND_IPC_SHMEM.emplace();
        auto resp = TXRX_RESPONSE.SEND_IPC_SHMEM->get_future();
        bool res =
            txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM, j.dump());

        if (res) {
            res = resp.get();
        }

        TXRX_RESPONSE.SEND_IPC_SHMEM.reset();
        if (res) {
            return true;
        } else {
            critlog("failed to send ipc shared memory.");
            return false;
        }
    } catch (const std::exception &e) {
        critlog("failed to send ipc shared memory. Why:");
        critlog(e.what());
        return false;
    }
}
}; // namespace PDJE_IPC