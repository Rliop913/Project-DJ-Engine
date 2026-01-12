#pragma once
#include "PDJE_Judge_Init.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_PreProcess.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
namespace PDJE_JUDGE {
class Match {
  private:
    PreProcess                  *pre;
    Judge_Init                  *init;
    P_NOTE_VEC                   found_list;
    std::vector<mouse_btn_event> mouse_btn_ev_queue;
    /** @brief Parse mouse-specific input and enqueue events. */
    void
    ParseMouse(const BITMASK ev, RAIL_KEY::KB_MOUSE &key);

  public:
    void
    Work(const LOCAL_TIME  input_time,
         const P_NOTE_VEC &note_list,
         const uint64_t    railid,
         const bool        isPressed);

    template <PDJE_Dev_Type D>
    /** @brief Handle a single input log entry for the given device type. */
    void
    UseEvent(const PDJE_Input_Log &ilog);

    void
    UseEvent(const PDJE_MIDI::MIDI_EV &ilog);

    Match(PreProcess *preproc, Judge_Init *judge_init)
    {
        init = judge_init;
        pre  = preproc;
        mouse_btn_ev_queue.reserve(7);
    }
    ~Match() = default;
};
}; // namespace PDJE_JUDGE