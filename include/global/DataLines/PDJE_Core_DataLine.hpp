#pragma once

#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_SYNC_CORE.hpp"
#include <atomic>
#include <cstdint>
// #include "MusicControlPanel.hpp"
/**
 * @brief data line for transmission with other mosudles.
 * all datas are pointer. use carefully.
 */
struct PDJE_API PDJE_CORE_DATA_LINE {
    unsigned long long         *nowCursor       = nullptr;
    unsigned long long         *maxCursor       = nullptr;
    float                      *preRenderedData = nullptr;
    std::atomic<audioSyncData> *syncD           = nullptr;
};