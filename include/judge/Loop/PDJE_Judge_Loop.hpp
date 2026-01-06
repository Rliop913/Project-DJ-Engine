#pragma once
#include "InputParser.hpp"
#include "Input_State.hpp"

#include "PDJE_Highres_Clock.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_Judge_Loop_Structs.hpp"
#include "PDJE_Match.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_PreProcess.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
#include <optional>
#include <vector>
namespace PDJE_JUDGE {

/** @brief Internal judge loop that consumes inputs, matches notes, and emits
 * callbacks. */
class Judge_Loop {

  private:
    EV_Thread Event_Controls;

  private:
    Judge_Init               *init_datas;
    PDJE_HIGHRES_CLOCK::CLOCK clock_root;
    PreProcess                pre;

    Match match;

  public:
    /** @brief Stop use/miss worker threads. */
    void
    EndEventLoop();
    /** @brief Start use/miss worker threads. */
    void
    StartEventLoop();
    /** @brief Main loop that polls inputs and judges notes. */
    void
    loop();

    std::atomic<bool> loop_switch;
    /** @brief Construct loop with initialized data sources. */
    Judge_Loop(Judge_Init &inits);
    ~Judge_Loop() = default;
};

}; // namespace PDJE_JUDGE
