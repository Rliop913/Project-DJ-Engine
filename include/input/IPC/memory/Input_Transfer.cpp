#include "Input_Transfer.hpp"

namespace PDJE_IPC {

void
PDJE_Input_Transfer::SetHmacEngine()
{
    hmacEngine = Botan::MessageAuthenticationCode::create("HMAC(SHA-256)");
    if (!hmacEngine) {
        critlog("cannot create HMAC(SHA-256).");
        return;
    }
    hmacEngine->set_key(metadata.psk.psk);
}
void
PDJE_Input_Transfer::SetHmac()
{
    if (!hmacEngine) {
        critlog("Failed to Set Hmac. hmac is invalid.");
        return;
    }
    try {
        hmacEngine->update(reinterpret_cast<const uint8_t *>(datas.data()),
                           (*length.ptr) * sizeof(PDJE_Input_Log));
        std::memcpy(
            hmac.ptr, hmacEngine->final().data(), hmacEngine->output_length());
    } catch (const std::exception &e) {
        critlog("caused exception on Setting hmac. What: ");
        critlog(e.what());
    }
}
bool
PDJE_Input_Transfer::VerifyHmac()
{
    if (!hmacEngine) {
        critlog("Failed to Verify Hmac. hmac is invalid.");
        return false;
    }
    try {
        hmacEngine->update(reinterpret_cast<const uint8_t *>(datas.data()),
                           (*length.ptr) * sizeof(PDJE_Input_Log));
        return Botan::constant_time_compare(
            hmacEngine->final().data(), hmac.ptr, hmacEngine->output_length());
    } catch (const std::exception &e) {
        critlog("caused exception on VerityHmac. What: ");
        critlog(e.what());
        return false;
    }
}

std::string
PDJE_Input_Transfer::GetMetaDatas()
{
    nj out;
    out["MAX_LENGTH"]              = metadata.max_length;
    out["LENNAME"]                 = metadata.lenname.string();
    out["BODYNAME"]                = metadata.bodyname.string();
    out["HMACNAME"]                = metadata.hmacname.string();
    out["DATA_REQUEST_EVENT_NAME"] = metadata.data_request_event_name.string();
    out["DATA_STORED_EVENT_NAME"]  = metadata.data_stored_event_name.string();

    out["PSK"] = metadata.psk.Encode();
    return out.dump();
}
PDJE_Input_Transfer::PDJE_Input_Transfer(
    const std::string &metajson) // subprocess init
{
    nj meta             = nj::parse(metajson);
    metadata.max_length = meta["MAX_LENGTH"].get<uint32_t>();
    metadata.lenname    = meta["LENNAME"].get<MNAME>();
    metadata.bodyname   = meta["BODYNAME"].get<MNAME>();
    metadata.hmacname   = meta["HMACNAME"].get<MNAME>();
    metadata.data_request_event_name =
        meta["DATA_REQUEST_EVENT_NAME"].get<MNAME>();
    metadata.data_stored_event_name =
        meta["DATA_STORED_EVENT_NAME"].get<MNAME>();

    if (!metadata.psk.Decode(meta["PSK"].get<std::string>())) {
        throw std::runtime_error("failed to decode psk.");
    }
    datas.reserve(metadata.max_length);
    subBuffer.reserve(metadata.max_length);
    length.GetIPCSharedMemory(metadata.lenname, 1);
    body.GetIPCSharedMemory(metadata.bodyname, metadata.max_length);
    hmac.GetIPCSharedMemory(metadata.hmacname, 32);
    req_event.ClientInit(metadata.data_request_event_name);
    stored_event.ClientInit(metadata.data_stored_event_name);
    SetHmacEngine();
}
PDJE_Input_Transfer::PDJE_Input_Transfer(
    const Input_Transfer_Metadata &metad) // mainprocess init
{
    metadata = metad;
    datas.reserve(metadata.max_length);
    subBuffer.reserve(metadata.max_length);
    length.MakeIPCSharedMemory(metadata.lenname, 1);
    body.MakeIPCSharedMemory(metadata.bodyname, metadata.max_length);
    hmac.MakeIPCSharedMemory(metadata.hmacname, 32);
    req_event.HostInit(metadata.data_request_event_name);
    stored_event.HostInit(metadata.data_stored_event_name);
    metadata.psk.Gen();

    SetHmacEngine();
}

PDJE_Input_Transfer::PDJE_Input_Transfer(const uint32_t max_length)
{
    datas.reserve(max_length);
    subBuffer.reserve(max_length);
}

void
PDJE_Input_Transfer::SendManageWorker()
{
    sendworker_switch = true;
    sendworker        = std::thread([this]() {
        while (sendworker_switch) {
            try {
                Send();
            } catch (const std::exception &e) {
                critlog("caught exception on SendManageWorker. What: ");
                critlog(e.what());
            }
        }
    });
}

PDJE_Input_Transfer::~PDJE_Input_Transfer()
{
    sendworker_switch = false;
    if (sendworker) {
        if (sendworker->joinable()) {
            sendworker->join();
        }
    }
}

}; // namespace PDJE_IPC