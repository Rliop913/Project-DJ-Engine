#pragma once

#include <cstdint>
#include <optional>

#include "Input_State.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Judge_Loop.hpp"
#include <atomic>
#include <thread>
#include <unordered_map>
#include <vector>
namespace PDJE_JUDGE {
enum JUDGE_STATUS {
    OK                   = 0,
    CORE_LINE_IS_MISSING = 1,
    INPUT_LINE_IS_MISSING,
    EVENT_RULE_IS_EMPTY,
    INPUT_RULE_IS_EMPTY,
    NOTE_OBJECT_IS_MISSING,
};

class PDJE_API JUDGE {
  private: // cached values
    std::optional<Judge_Loop> loop_obj;

  private:
    // thread relates
    std::optional<std::thread> loop;

    JUDGE_STATUS status = JUDGE_STATUS::OK;

  public:
    Judge_Init inits;
    JUDGE_STATUS
    Start();
    void
    End();
    JUDGE_STATUS
    CheckStatus();
    JUDGE();
    ~JUDGE() = default;
};
}; // namespace PDJE_JUDGE