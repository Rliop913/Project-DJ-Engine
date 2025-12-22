#include "ConfigLayer.hpp"
#include "NameGen.hpp"
namespace PDJE_INPUT_LAYER {
void
Config_TX_RX::SetTXRX_Features()
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
                              critlog("Device config failed. Why:");
                              critlog(msg);
                              TXRX_RESPONSE.SEND_IPC_SHMEM->set_value(false);
                          }
                      });
}

bool
Config_TX_RX::CheckHealth()
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
Config_TX_RX::GetDevices()
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

using namespace PDJE_CRYPTO;
Config_TX_RX::Config_TX_RX()
{

    psk.Gen();
    RANDOM_GEN  gen;
    std::string mF = gen.Gen("pdjeconfigtxrx_mf");
    std::string lF = gen.Gen("pdjeconfigtxrx_lf");
    std::string mS = gen.Gen("pdjeconfigtxrx_ms");
    std::string lS = gen.Gen("pdjeconfigtxrx_ls");

    txrx.emplace(psk,
                 PDJE_IPC::MNAME(mF),
                 PDJE_IPC::MNAME(lF),
                 PDJE_IPC::MNAME(mS),
                 PDJE_IPC::MNAME(lS),
                 true);
    SetTXRX_Features();
    sub = std::make_shared<SubProcCTRL>();
    sub->OpenProcess(psk,
                     PDJE_IPC::MNAME(mF),
                     PDJE_IPC::MNAME(lF),
                     PDJE_IPC::MNAME(mS),
                     PDJE_IPC::MNAME(lS));
    txrx->Listen();
}

}; // namespace PDJE_INPUT_LAYER