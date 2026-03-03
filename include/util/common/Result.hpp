#pragma once

#include "util/common/Status.hpp"

#include <utility>

namespace PDJE_UTIL::common {

template <class T> class Result {
  public:
    static Result
    success(T value)
    {
        return Result(std::move(value), {});
    }

    static Result
    failure(Status status)
    {
        return Result(std::move(status));
    }

    bool
    ok() const noexcept
    {
        return status_.ok();
    }

    const T &
    value() const &
    {
        return value_;
    }

    T &
    value() &
    {
        return value_;
    }

    T &&
    value() &&
    {
        return std::move(value_);
    }

    const Status &
    status() const noexcept
    {
        return status_;
    }

  private:
    explicit Result(T value, Status status)
        : value_(std::move(value)), status_(std::move(status))
    {
    }

    explicit Result(Status status) : value_(), status_(std::move(status)) {}

    T      value_;
    Status status_;
};

template <> class Result<void> {
  public:
    static Result
    success()
    {
        return Result({});
    }

    static Result
    failure(Status status)
    {
        return Result(std::move(status));
    }

    bool
    ok() const noexcept
    {
        return status_.ok();
    }

    const Status &
    status() const noexcept
    {
        return status_;
    }

  private:
    explicit Result(Status status) : status_(std::move(status)) {}

    Status status_;
};

} // namespace PDJE_UTIL::common
