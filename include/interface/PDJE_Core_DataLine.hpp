#pragma once

#include "PDJE_EXPORT_SETTER.hpp"

#include "MusicControlPannel.hpp"
/**
 * @brief data line for transmission with other mosudles.
 * all datas are pointer. use carefully.
 */
struct PDJE_API PDJE_CORE_DATA_LINE {
    unsigned long long *used_frame      = nullptr;
    unsigned long long *nowCursor       = nullptr;
    unsigned long long *maxCursor       = nullptr;
    float              *preRenderedData = nullptr;

    FXControlPannel    *fx   = nullptr;
    MusicControlPannel *musp = nullptr;
};