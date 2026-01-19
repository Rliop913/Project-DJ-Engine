#include "MetadataTXRX.hpp"

namespace PDJE_IPC {

bool
MetadataTXRX::QueryConfig(const std::string &dumped_json)
{
    TXRX_RESPONSE.DEVICE_CONFIG.emplace();
    auto resp = TXRX_RESPONSE.DEVICE_CONFIG->get_future();
    bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG, dumped_json);
    if (res) {
        res = resp.get();
    }

    TXRX_RESPONSE.DEVICE_CONFIG.reset();
    if (res) {
        return EndTransmission();
    }

    else {
        critlog("query configure failed.");
        return false;
    }
}

bool
MetadataTXRX::SendInputTransfer(PDJE_Input_Transfer &trsf)
{

    try {
        TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM.emplace();
        auto resp = TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM->get_future();
        bool res =
            txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
                       trsf.GetMetaDatas());

        if (res) {
            res = resp.get();
        }

        TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM.reset();
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
std::stringstream
MetadataTXRX::GenTXRX()
{
    if (!psk.Gen()) {
        return {};
    }
    PDJE_CRYPTO::RANDOM_GEN rg;
    PDJE_IPC::MNAME         mfirst  = rg.Gen("PDJE_TXRX_F_");
    PDJE_IPC::MNAME         lfirst  = rg.Gen("PDJE_TXRX_LOCK_F_");
    PDJE_IPC::MNAME         msecond = rg.Gen("PDJE_TXRX_S_");
    PDJE_IPC::MNAME         lsecond = rg.Gen("PDJE_TXRX_LOCK_S_");

    txrx.emplace(psk, mfirst, lfirst, msecond, lsecond, true);
    SetTXRX_Features();
    std::stringstream ss;

    ss << psk.Encode();
    ss << " ";
    ss << mfirst.string();
    ss << " ";
    ss << lfirst.string();
    ss << " ";
    ss << msecond.string();
    ss << " ";
    ss << lsecond.string();
    return ss;
}

bool
MetadataTXRX::QueryHealth()
{
    TXRX_RESPONSE.HEALTH_CHECK.emplace();
    auto resp = TXRX_RESPONSE.HEALTH_CHECK->get_future();
    bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK, "");

    if (res) {
        res = resp.get();
    }

    TXRX_RESPONSE.HEALTH_CHECK.reset();
    if (res)
        return true;
    else {
        critlog("health check failed.");
        return false;
    }
}
std::vector<DeviceData>
MetadataTXRX::QueryDevices()
{
    TXRX_RESPONSE.DEVICE_LIST.emplace();
    auto resp = TXRX_RESPONSE.DEVICE_LIST->get_future();
    bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_LIST, "");

    if (!res) {
        TXRX_RESPONSE.DEVICE_LIST.reset();
        critlog("failed to request device list.");
        return {};
    }
    return resp.get();
}

bool
MetadataTXRX::Kill()
{
    TXRX_RESPONSE.KILL.emplace();
    auto resp = TXRX_RESPONSE.KILL->get_future();
    bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL, "");

    if (res) {
        res = resp.get();
    }

    TXRX_RESPONSE.KILL.reset();
    txrx.reset();
    if (res)
        return true;
    else {
        critlog("failed to send kill signal.");
        return false;
    }
}
bool
MetadataTXRX::SendIPCSharedMemory(const uint64_t     mem_length,
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

bool
MetadataTXRX::EndTransmission()
{
    TXRX_RESPONSE.STOP.emplace();
    auto resp = TXRX_RESPONSE.STOP->get_future();
    bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_STOP, "");
    if (res) {
        res = resp.get();
    }
    TXRX_RESPONSE.STOP.reset();
    txrx.reset();
    return res;
}

}; // namespace PDJE_IPC