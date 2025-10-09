#include "PDJE_Judge.hpp"

PDJE_JUDGE::PDJE_JUDGE(const PDJE_CORE_DATA_LINE& core_line, const PDJE_INPUT_DATA_LINE& input_line)
{
    if(core_line.maxCursor != nullptr &&
        core_line.nowCursor != nullptr &&
        core_line.preRenderedData != nullptr &&
        core_line.used_frame != nullptr){
            core = core_line;
        }
    if(input_line.input_arena != nullptr && input_line.id_name_conv != nullptr){
        input = input_line;
    }
}