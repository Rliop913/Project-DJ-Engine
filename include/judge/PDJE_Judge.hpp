#pragma once

#include <cstdint>
#include <optional>

#include "Input_State.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Judge_Loop.hpp"
#include <atomic>
#include <thread>
#include <unordered_map>
#include <vector>
namespace PDJE_JUDGE {
/** @brief Judge runtime status codes. */
enum JUDGE_STATUS {
    OK                   = 0,
    CORE_LINE_IS_MISSING = 1,
    INPUT_LINE_IS_MISSING,
    EVENT_RULE_IS_EMPTY,
    INPUT_RULE_IS_EMPTY,
    NOTE_OBJECT_IS_MISSING,
    ALREADY_RUNNING = 6,
};

/** @brief Judge controller that owns initialization data and the event loop. */
class PDJE_API JUDGE {
  private:
    std::optional<Judge_Loop> loop_obj;
  private:
    // thread relates
    std::optional<std::jthread> loop;

  public:
    Judge_Init inits;
    // Gameplay orchestration belongs to PDJE_GAMEPLAY::FACADE, not Judge.
    /** @brief Validate init data and start the judge event loop thread. */
    JUDGE_STATUS
    Start();
    /** @brief Stop the event loop and release cached init data. */
    void
    End();

    /** @brief Gate new judgments and wait for direct callbacks/note mutation.
     * Polling and committed use/miss callback delivery continue. All lifecycle
     * methods (including queries) must be serialized with Start/End on the
     * control thread; never invoke them from any Judge callback.
     * Suspend/Resume return false without a runtime, and are idempotent.
     */
    bool
    SuspendJudgments();
    /** @brief Start missing event workers, then ungate with a raw input cutoff.
     * Does not start playback or change Core sync. The facade must invalidate
     * stopped Core sync before resuming. Failure leaves production suspended.
     */
    bool
    ResumeJudgments();
    /** @brief Join event workers, discard queued jobs and reset note progress.
     * A runtime remains installed, polling and suspended, with event workers
     * stopped until ResumeJudgments(). Without a runtime only notes are reset.
     * Requires configured notes; preserves rails/rules/callbacks/borrowed
     * lines.
     */
    bool
    ResetForRestart();
    /** @brief Whether an installed runtime has gated judgment production. */
    bool
    IsSuspended() const;

    /** @brief Whether a runtime is installed. Serialize with Start()/End(). */
    bool
    IsRunning() const noexcept
    {
        return loop.has_value() || loop_obj.has_value();
    }

    /** @brief Create a judge instance. */
    JUDGE();
    ~JUDGE() noexcept{
        End();
    }
};
}; // namespace PDJE_JUDGE
