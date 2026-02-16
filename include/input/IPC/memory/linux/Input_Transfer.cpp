#include "Input_Transfer.hpp"
namespace PDJE_IPC {

void
PDJE_Input_Transfer::Receive()
{
    datas = *adbf.Get();
}
PDJE_Input_Transfer::PDJE_Input_Transfer(const uint32_t max_length)
    : adbf(max_length)
{
}
void
PDJE_Input_Transfer::Write(const PDJE_Input_Log &input)
{
    adbf.Write(input);
}

PDJE_Input_Transfer::~PDJE_Input_Transfer()
{
}

}; // namespace PDJE_IPC