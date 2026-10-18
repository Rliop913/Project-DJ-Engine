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
    uint64_t                  *nowCursor       = nullptr;
    uint64_t                  *maxCursor       = nullptr;
    float                      *preRenderedData = nullptr;
    std::atomic<audioSyncData> *syncD           = nullptr;
};