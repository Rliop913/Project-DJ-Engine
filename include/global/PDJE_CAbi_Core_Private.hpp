#pragma once

#ifdef __cplusplus

#include <cstdint>

struct PDJE_EngineHandleV1 {
    void               *engine       = nullptr;
    uint64_t           *now_cursor    = nullptr;
    uint64_t           *max_cursor    = nullptr;
    float              *pre_rendered = nullptr;
    void               *sync_data    = nullptr;
};

namespace PDJE_CABI {

inline const PDJE_EngineHandleV1 *
BorrowCoreDataLine(const PDJE_EngineHandleV1 *engine) noexcept
{
    return engine;
}

} // namespace PDJE_CABI

#endif
