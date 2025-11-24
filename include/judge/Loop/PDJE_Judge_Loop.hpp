#pragma once
#include "Input_State.hpp"

#include "PDJE_Highres_Clock.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_Judge_Loop_Structs.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
#include <vector>

namespace PDJE_JUDGE {

class Judge_Loop {

  private:
    EV_Thread Event_Controls;

    Queues Event_Datas;

  private: // cached values
    std::pair<PDJE_Input_Log *, uint64_t> input_log;

    LoopCached Cached;

    Judge_Init               *init_datas;
    PDJE_HIGHRES_CLOCK::CLOCK clock_root;
    inline void
    Cut();
    bool
    PreProcess();
    void
    ParseMouse(INPUT_RULE &rule, const BITMASK ev);
    template <PDJE_Dev_Type D>
    void
    UseEvent(const PDJE_Input_Log &ilog);
    bool
    FindDevSetting(const INPUT_RULE &rule, INPUT_SETTING &setting);
    void
    Match(const LOCAL_TIME  input_time,
          const P_NOTE_VEC &note_list,
          const uint64_t    railid,
          const bool        isPressed);

  public:
    void
    EndEventLoop();
    void
    StartEventLoop();
    void
    loop();

    std::atomic<bool> loop_switch;
    Judge_Loop(Judge_Init &inits);
    ~Judge_Loop() = default;
};

}; // namespace PDJE_JUDGE