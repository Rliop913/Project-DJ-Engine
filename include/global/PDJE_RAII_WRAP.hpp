#pragma once

#include "PDJE_LOG_SETTER.hpp"
#include <optional>
#include <utility>
namespace PDJE_RAII {
template <typename T, typename Deleter> class RAII {

  private:
    Deleter deleter;

  public:
    RAII() = default;

    RAII(const RAII &) = delete;
    RAII &
    operator=(const RAII &) = delete;

    RAII(RAII &&other) noexcept
        : val(std::move(other.val)), deleter(std::move(other.deleter))
    {
        other.val = std::nullopt;
    }

    RAII &
    operator=(RAII &&other) noexcept
    {
        if (this != &other) {
            reset();
            val     = std::move(other.val);
            deleter = std::move(other.deleter);
            other.val.reset();
        }
        return *this;
    }

    ~RAII() noexcept
    {
        reset();
    }

    void
    reset() noexcept
    {
        if (val) {
            deleter(val.value());
            val.reset();
        }
    }

    T
    get() const
    {
        return val.value();
    }
    explicit
    operator bool() const noexcept
    {
        return val.has_value();
    }
    std::optional<T> val;
};
}; // namespace PDJE_RAII