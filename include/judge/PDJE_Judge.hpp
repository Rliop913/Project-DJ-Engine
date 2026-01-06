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
/** @brief Judge runtime status codes. */
enum JUDGE_STATUS {
    OK                   = 0,
    CORE_LINE_IS_MISSING = 1,
    INPUT_LINE_IS_MISSING,
    EVENT_RULE_IS_EMPTY,
    INPUT_RULE_IS_EMPTY,
    NOTE_OBJECT_IS_MISSING,
};

/** @brief Judge controller that owns initialization data and the event loop. */
class PDJE_API JUDGE {
  private:
    std::optional<Judge_Loop> loop_obj;

  private:
    // thread relates
    std::optional<std::thread> loop;

  public:
    Judge_Init inits;
    /** @brief Validate init data and start the judge event loop thread. */
    JUDGE_STATUS
    Start();
    /** @brief Stop the event loop and release cached init data. */
    void
    End();

    /** @brief Create a judge instance and initialize logging. */
    JUDGE();
    ~JUDGE() = default;
};
}; // namespace PDJE_JUDGE
