#include "PDJE_Judge.hpp"

PDJE_JUDGE::PDJE_JUDGE(const PDJE_CORE_DATA_LINE& core_line)
{
    if(core_line.maxCursor != nullptr &&
        core_line.nowCursor != nullptr &&
        core_line.preRenderedData != nullptr &&
        core_line.used_frame != nullptr){
            core = core_line;
        }
}