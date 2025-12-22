#include "Input_Transfer.hpp"
namespace PDJE_IPC {
void
PDJE_Input_Transfer::Send()
{
    locker.lock();
    (*length.ptr) = datas.size();
    SetHmac();
    std::memcpy(body.ptr, datas.data(), sizeof(PDJE_Input_Log) * (*length.ptr));
    locker.unlock();
}

void
PDJE_Input_Transfer::Receive()
{
    locker.lock();
    if (*length.ptr != 0) {
        datas.resize(*length.ptr);
        std::memcpy(
            datas.data(), body.ptr, sizeof(PDJE_Input_Log) * (*length.ptr));
        if (!VerifyHmac()) {
            datas.clear();
        }
        *length.ptr = 0;
    }
    locker.unlock();
}
}; // namespace PDJE_IPC