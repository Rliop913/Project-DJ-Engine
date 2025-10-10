#pragma once

#include <optional>

#include "PDJE_Core_DataLine.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <atomic>
#include <thread>
#include <vector>
namespace PDJE_JUDGE {
enum JUDGE_STATUS {
    OK                   = 0,
    CORE_LINE_IS_MISSING = 1,
    INPUT_LINE_IS_MISSING,
    NOTE_OBJECT_IS_MISSING,
};

class JUDGE {
  private:
    std::optional<PDJE_CORE_DATA_LINE>  core;
    std::optional<PDJE_INPUT_DATA_LINE> input;
    std::optional<OBJ>                  note_obj;
    std::optional<std::thread>          loop;
    JUDGE_STATUS                        status = JUDGE_STATUS::OK;
    std::atomic<bool>                   loop_switch;

    void
    Judge_Loop();

  public:
    void
    NoteObjectCollector(const std::string        noteType,
                        const std::string        noteDetail,
                        const std::string        firstArg,
                        const std::string        secondArg,
                        const std::string        thirdArg,
                        const unsigned long long Y_Axis,
                        const unsigned long long Y_Axis_2);

    void
    SetCoreLine(const PDJE_CORE_DATA_LINE &coreline);
    void
    SetInputLine(const PDJE_INPUT_DATA_LINE &inputline);

    JUDGE_STATUS
    Start();
    void
    End();
    JUDGE_STATUS
    CheckStatus();
    JUDGE()  = default;
    ~JUDGE() = default;
};
}; // namespace PDJE_JUDGE