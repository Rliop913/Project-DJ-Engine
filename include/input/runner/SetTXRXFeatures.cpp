#include "SubProcess.hpp"

namespace PDJE_IPC {
using namespace SUBPROC;

TXRXListener::TXRXListener(PDJE_CRYPTO::PSK      &psk,
                           const PDJE_IPC::MNAME &memFirst,
                           const PDJE_IPC::MNAME &firstLock,
                           const PDJE_IPC::MNAME &memSecond,
                           const PDJE_IPC::MNAME &secondLock)
{
    startlog();
    txrx.emplace(psk, memFirst, firstLock, memSecond, secondLock, false);

    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL,
                      [this](const std::string &msg) {
                          KillCheck = true;
                          txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL, "OK");
                          txrx->StopListen();
                      });

    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::TXRX_STOP,
                      [this](const std::string &msg) {
                          txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_STOP, "OK");
                          txrx->StopListen();
                      });

    txrx->AddFunction(
        PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK, [this](const std::string &msg) {
            txrx->Send(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK, "OK");
        });

    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::DEVICE_LIST,
                      [this](const std::string &msg) {
                          txrx->Send(PDJE_CRYPTO::DEVICE_LIST, ListDev());
                      });

    txrx->AddFunction(
        PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG, [this](const std::string &msg) {
            try {
                configed_devices.clear();
                auto nj = nlohmann::json::parse(msg);
                for (const auto &i : nj["body"]) {
                    DeviceData dd;
                    dd.device_specific_id = i.at("id").get<std::string>();
                    dd.Name               = i.at("name").get<std::string>();

                    std::string tp = i.at("type").get<std::string>();
                    if (tp == "KEYBOARD") {
                        dd.Type = PDJE_Dev_Type::KEYBOARD;
                    } else if (tp == "MOUSE") {
                        dd.Type = PDJE_Dev_Type::MOUSE;
                    } else {
                        continue;
                    }
                    configed_devices.push_back(dd);
                }
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG, "OK");
            } catch (const std::exception &e) {

                std::string errlog =
                    "INVALID_JSON. why:" + std::string(e.what());
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG, errlog);

                critlog("failed to config device data. WHY: ");
                critlog(e.what());
                critlog("received json: ");
                critlog(msg);
            }
        });

    txrx->AddFunction(
        PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM,
        [this](const std::string &msg) {
            try {
                auto nj = nlohmann::json::parse(msg);

                if (!RecvIPCSharedMem(nj.at("PATH").get<std::string>(),
                                      nj.at("DATATYPE").get<std::string>(),
                                      nj.at("COUNT").get<uint64_t>())) {
                    throw std::runtime_error(
                        "failed to receive ipc shared memory.");
                }
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM, "OK");
            } catch (const std::exception &e) {
                std::string errlog =
                    "INVALID_JSON. why:" + std::string(e.what());
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM, errlog);
                critlog("failed to config device data. WHY: ");
                critlog(e.what());
                critlog("received json: ");
                critlog(msg);
            }
        });
    txrx->AddFunction(
        PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
        [this](const std::string &msg) {
            try {
                input_buffer.emplace(msg);
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
                           "OK");
            } catch (const std::exception &e) {
                std::string errlog =
                    "INVALID_JSON. why:" + std::string(e.what());
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
                           errlog);
                critlog("failed to config device data. WHY: ");
                critlog(e.what());
                critlog("received json: ");
                critlog(msg);
            }
        });
}
}; // namespace PDJE_IPC