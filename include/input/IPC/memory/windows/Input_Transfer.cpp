#include "Input_Transfer.hpp"
namespace PDJE_IPC {
void
PDJE_Input_Transfer::Send()
{
    req_event.Wait(1000);

    {
        std::lock_guard<std::mutex> g(local_locker);
        datas.clear();
        if (!subBuffer.empty()) {
            datas.swap(subBuffer);
        }
    }

    (*length.ptr) = datas.size();
    if (!datas.empty()) {
        SetHmac();
        std::memcpy(
            body.ptr, datas.data(), sizeof(PDJE_Input_Log) * (*length.ptr));
    }
    stored_event.Wake();
}

void
PDJE_Input_Transfer::Receive()
{
    req_event.Wake();
    stored_event.Wait();
    if (*length.ptr != 0) {
        datas.resize(*length.ptr);
        std::memcpy(
            datas.data(), body.ptr, sizeof(PDJE_Input_Log) * (*length.ptr));
        if (!VerifyHmac()) {
            datas.clear();
        }
        *length.ptr = 0;
    } else {
        datas.clear();
    }
}

void
PDJE_Input_Transfer::Write(const PDJE_Input_Log &input)
{
    std::lock_guard<std::mutex> g(local_locker);
    if (subBuffer.size() < metadata.max_length) {
        subBuffer.push_back(input);
    }
}

}; // namespace PDJE_IPC