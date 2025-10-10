#include "PDJE_Judge.hpp"

void
PDJE_JUDGE::SetCoreLine(const PDJE_CORE_DATA_LINE &coreline)
{
    if (coreline.maxCursor != nullptr && coreline.nowCursor != nullptr &&
        coreline.preRenderedData != nullptr && coreline.used_frame != nullptr) {
        core = coreline;
    }
}

void
PDJE_JUDGE::SetInputLine(const PDJE_INPUT_DATA_LINE &inputline)
{
    if (inputline.input_arena != nullptr && inputline.id_name_conv != nullptr) {
        input = inputline;
    }
}

void
PDJE_JUDGE::NoteObjectCollector(const std::string        noteType,
                                const std::string        noteDetail,
                                const std::string        firstArg,
                                const std::string        secondArg,
                                const std::string        thirdArg,
                                const unsigned long long Y_Axis,
                                const unsigned long long Y_Axis_2)
{
}

JUDGE_STATUS
PDJE_JUDGE::Start()
{
    if (!core.has_value()) {
        return JUDGE_STATUS::CORE_LINE_IS_MISSING;
    }
    if (!input.has_value()) {
        return JUDGE_STATUS::INPUT_LINE_IS_MISSING;
    }
    // todo - impl
}