#pragma once

namespace PDJE_UTIL::function {

class CacheContext {
  public:
    CacheContext() = default;
    CacheContext(const CacheContext &) = delete;
    CacheContext &operator=(const CacheContext &) = delete;
    CacheContext(CacheContext &&) noexcept = default;
    CacheContext &operator=(CacheContext &&) noexcept = default;
    ~CacheContext() = default;
};

struct EvalOptions {
    CacheContext *cache = nullptr;
};

} // namespace PDJE_UTIL::function
