#pragma once
#include "PDJE_Buffer.hpp"
#include "PDJE_Input_Log.hpp"
#include <nlohmann/json.hpp>
using nj = nlohmann::json;

namespace PDJE_IPC {
class PDJE_API PDJE_Input_Transfer {
  private:
  public:
    std::vector<PDJE_Input_Log>          datas;
    Atomic_Double_Buffer<PDJE_Input_Log> adbf;
    PDJE_Input_Transfer(const uint32_t max_length);

    void
    Write(const PDJE_Input_Log &input);
    void
    Receive();
    ~PDJE_Input_Transfer();
};

}; // namespace PDJE_IPC