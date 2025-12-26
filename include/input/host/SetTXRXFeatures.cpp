#include "MainProcess.hpp"

namespace PDJE_IPC {
using namespace MAINPROC;
void
TXRXTransport::SetTXRX_Features()
{
    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK,
                      [this](const std::string &msg) {
                          if (msg == "OK") {
                              TXRX_RESPONSE.HEALTH_CHECK->set_value(true);
                          } else {
                              TXRX_RESPONSE.HEALTH_CHECK->set_value(false);
                          }
                      });
    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::TXRX_STOP,
                      [this](const std::string &msg) {
                          if (msg == "OK") {
                              TXRX_RESPONSE.STOP->set_value(true);
                          } else {
                              TXRX_RESPONSE.STOP->set_value(false);
                          }
                      });
    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL,
                      [this](const std::string &msg) {
                          if (msg == "OK") {
                              TXRX_RESPONSE.KILL->set_value(true);
                          } else {
                              TXRX_RESPONSE.KILL->set_value(false);
                          }
                      });
    txrx->AddFunction(
        PDJE_CRYPTO::TXRXHEADER::DEVICE_LIST, [this](const std::string &msg) {
            try {

                nj                      jj = nj::parse(msg);
                std::vector<DeviceData> dlist;
                for (const auto &i : jj["body"]) {
                    DeviceData dd;
                    dd.device_specific_id = i.at("id").get<std::string>();
                    dd.Name               = i.at("name").get<std::string>();
                    std::string tp        = i.at("type").get<std::string>();
                    if (tp == "KEYBOARD") {
                        dd.Type = PDJE_Dev_Type::KEYBOARD;
                    } else if (tp == "MOUSE") {
                        dd.Type = PDJE_Dev_Type::MOUSE;
                    } else if (tp == "MIDI") {
                        dd.Type = PDJE_Dev_Type::MIDI;
                    } else if (tp == "HID") {
                        dd.Type = PDJE_Dev_Type::HID;
                    } else {
                        continue;
                    }
                    dlist.push_back(dd);
                }
                TXRX_RESPONSE.DEVICE_LIST->set_value(dlist);
            } catch (const std::exception &e) {
                critlog("failed to list devices. Why: ");
                critlog(e.what());
                critlog("JSON dump: ");
                critlog(msg);
                TXRX_RESPONSE.DEVICE_LIST->set_value({});
            }
        });
    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG,
                      [this](const std::string &msg) {
                          if (msg == "OK") {
                              TXRX_RESPONSE.DEVICE_CONFIG->set_value(true);
                          } else {
                              critlog("Device config failed. Why:");
                              critlog(msg);
                              TXRX_RESPONSE.DEVICE_CONFIG->set_value(false);
                          }
                      });

    txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM,
                      [this](const std::string &msg) {
                          if (msg == "OK") {
                              TXRX_RESPONSE.SEND_IPC_SHMEM->set_value(true);
                          } else {
                              critlog("Send IPC SHMEM failed. Why:");
                              critlog(msg);
                              TXRX_RESPONSE.SEND_IPC_SHMEM->set_value(false);
                          }
                      });
    txrx->AddFunction(
        PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
        [this](const std::string &msg) {
            if (msg == "OK") {
                TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM->set_value(true);
            } else {
                critlog("Send Input Transfer SHMEM failed. Why:");
                critlog(msg);
                TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM->set_value(false);
            }
        });
}
}; // namespace PDJE_IPC