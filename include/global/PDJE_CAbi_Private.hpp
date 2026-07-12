#pragma once

#ifdef __cplusplus

#include "PDJE_LOG_SETTER.hpp"

#include <cstddef>
#include <exception>
#include <string>
#include <type_traits>
#include <utility>

namespace PDJE_CABI {

template <typename Error, typename Fn>
auto
Guard(const char *context, const Error internal_error, Fn &&fn) noexcept
    -> std::invoke_result_t<Fn>
{
    using Result = std::invoke_result_t<Fn>;
    try {
        return std::forward<Fn>(fn)();
    } catch (const std::exception &e) {
        critlog(context);
        critlog(e.what());
    } catch (...) {
        critlog(context);
    }
    return static_cast<Result>(internal_error);
}

template <typename Fn>
bool
GuardVoid(const char *context, Fn &&fn) noexcept
{
    try {
        std::forward<Fn>(fn)();
        return true;
    } catch (const std::exception &e) {
        critlog(context);
        critlog(e.what());
    } catch (...) {
        critlog(context);
    }
    return false;
}

template <typename Handle>
bool
PrepareHandleOutput(Handle **out_handle) noexcept
{
    if (out_handle == nullptr) {
        return false;
    }
    *out_handle = nullptr;
    return true;
}

template <typename T>
bool
StructIsCompatible(const T *value) noexcept
{
    return value != nullptr &&
           (value->struct_size == 0 || value->struct_size >= sizeof(T));
}

template <typename T>
void
ResetVersionedOutput(T *value) noexcept
{
    if (value == nullptr) {
        return;
    }
    *value             = {};
    value->struct_size = sizeof(T);
}

inline const char *
OptionalString(const char *value) noexcept
{
    return value != nullptr ? value : "";
}

template <typename View>
View
MakeStringView(const std::string &value) noexcept
{
    if (value.empty()) {
        return {};
    }
    return View{ value.c_str(), value.size() };
}

template <typename View>
View
MakeCountedStringView(const char *value, const std::size_t value_size) noexcept
{
    if (value == nullptr || value_size == 0) {
        return {};
    }
    return View{ value, value_size };
}

} // namespace PDJE_CABI

#endif
