#pragma once

#include <utility>

// Internal lifecycle helpers; not part of the public C ABI.
namespace PDJE_CABI {

// The flag is an API mutation guard, not proof that no runtime resources exist.
// end must contain exceptions and report whether cleanup completed.
template <typename End>
bool
EndJudgeRuntime(bool &running, End &&end)
{
    const bool ended = std::forward<End>(end)();
    if (ended) {
        running = false;
    } else {
        // Keep configuration/start blocked until cleanup succeeds.
        running = true;
    }
    return ended;
}

// Preserve ordinary prerequisite status returns. On an exception, clean up any
// partially started runtime before the outer C ABI guard translates the error.
// end must contain exceptions and report whether cleanup completed.
template <typename Start, typename End>
auto
StartJudgeRuntime(bool &running, Start &&start, End &&end)
    -> decltype(std::forward<Start>(start)())
{
    try {
        return std::forward<Start>(start)();
    } catch (...) {
        running = true; // A failed start can still own worker threads.
        EndJudgeRuntime(running, std::forward<End>(end));
        throw;
    }
}

} // namespace PDJE_CABI
