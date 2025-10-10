#include "PDJE_Judge.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <thread>
namespace PDJE_JUDGE {

void
JUDGE::SetCoreLine(const PDJE_CORE_DATA_LINE &coreline)
{
    if (coreline.maxCursor != nullptr && coreline.nowCursor != nullptr &&
        coreline.preRenderedData != nullptr && coreline.used_frame != nullptr) {
        core = coreline;
    }
}

void
JUDGE::SetInputLine(const PDJE_INPUT_DATA_LINE &inputline)
{
    if (inputline.input_arena != nullptr && inputline.id_name_conv != nullptr) {
        input = inputline;
    }
}

void
JUDGE::NoteObjectCollector(const std::string        noteType,
                           const std::string        noteDetail,
                           const std::string        firstArg,
                           const std::string        secondArg,
                           const std::string        thirdArg,
                           const unsigned long long Y_Axis,
                           const unsigned long long Y_Axis_2)
{
    if (!note_obj.has_value()) {
        note_obj.emplace();
    }
    auto tempobj = NOTE{ .type   = noteType,
                         .detail = noteDetail,
                         .first  = firstArg,
                         .second = secondArg,
                         .third  = thirdArg,
                         .pos    = 0,
                         .used   = false };

    tempobj.pos = Y_Axis;
    note_obj->Fill<IN>(tempobj);
    if (Y_Axis != 0) {
        tempobj.pos = Y_Axis_2;
        note_obj->Fill<OUT>(tempobj);
    }
}

JUDGE_STATUS
JUDGE::Start()
{
    if (!core.has_value()) {
        return JUDGE_STATUS::CORE_LINE_IS_MISSING;
    }
    if (!input.has_value()) {
        return JUDGE_STATUS::INPUT_LINE_IS_MISSING;
    }
    if (!note_obj.has_value()) {
        return JUDGE_STATUS::NOTE_OBJECT_IS_MISSING;
    }
    note_obj->Sort();
    loop_switch = true;
    loop.emplace([this]() { Judge_Loop(); });
    return JUDGE_STATUS::OK;
}

JUDGE_STATUS
JUDGE::CheckStatus()
{
    return status;
}

void
JUDGE::End()
{
    loop_switch = false;
    loop->join();
    core.reset();
    input.reset();
    note_obj.reset();
}

void
JUDGE::Judge_Loop()
{
    while (loop_switch) {
    }
}

}; // namespace PDJE_JUDGE