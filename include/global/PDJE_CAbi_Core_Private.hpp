#pragma once

#ifdef __cplusplus

struct PDJE_EngineHandleV1 {
    void               *engine       = nullptr;
    unsigned long long *now_cursor    = nullptr;
    unsigned long long *max_cursor    = nullptr;
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
