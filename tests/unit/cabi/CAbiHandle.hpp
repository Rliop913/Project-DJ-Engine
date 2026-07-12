#pragma once

#include <utility>

namespace PDJE_TEST {

template <typename Handle, auto Destroy> class CAbiHandle {
  public:
    CAbiHandle() = default;

    ~CAbiHandle()
    {
        reset();
    }

    CAbiHandle(const CAbiHandle &) = delete;
    CAbiHandle &
    operator=(const CAbiHandle &) = delete;

    CAbiHandle(CAbiHandle &&other) noexcept
        : value_(std::exchange(other.value_, nullptr))
    {
    }

    CAbiHandle &
    operator=(CAbiHandle &&other) noexcept
    {
        if (this != &other) {
            reset();
            value_ = std::exchange(other.value_, nullptr);
        }
        return *this;
    }

    Handle **
    put()
    {
        reset();
        return &value_;
    }

    Handle *
    get() const noexcept
    {
        return value_;
    }

    explicit
    operator bool() const noexcept
    {
        return value_ != nullptr;
    }

    operator Handle *() const noexcept
    {
        return value_;
    }

    void
    reset(Handle *value = nullptr) noexcept
    {
        if (value_ != nullptr) {
            Destroy(value_);
        }
        value_ = value;
    }

  private:
    Handle *value_ = nullptr;
};

} // namespace PDJE_TEST
