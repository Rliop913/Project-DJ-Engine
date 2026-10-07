#include "PDJE_GamePlay.hpp"
#include "PDJE_Input.hpp"
#include "PDJE_Judge.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "PDJE_interface.hpp"
#include <atomic>
#include <exception>
#include <functional>
#include <stdexcept>
#include <utility>

namespace PDJE_GAMEPLAY {

FACADE::FACADE(std::shared_ptr<PDJE>              core,
               std::shared_ptr<PDJE_Input>        input,
               std::shared_ptr<PDJE_JUDGE::JUDGE> judge)
    : core(std::move(core)), input(std::move(input)), judge(std::move(judge))
{
    if (!this->core || !this->input || !this->judge) {
        throw std::invalid_argument(
            "FACADE requires non-null Core, Input and Judge");
    }
}

std::shared_ptr<PDJE>
FACADE::GetCore() const noexcept
{
    return core;
}

std::shared_ptr<PDJE_Input>
FACADE::GetInput() const noexcept
{
    return input;
}

std::shared_ptr<PDJE_JUDGE::JUDGE>
FACADE::GetJudge() const noexcept
{
    return judge;
}

bool
FACADE::HasCurrentDataLines() const
{
    if (!core->player || !judge->inits.coreline || !judge->inits.inputline) {
        return false;
    }
    const auto core_line  = core->player->PullOutDataLine();
    const auto input_line = input->PullOutDataLine();
    return core_line.syncD && judge->inits.coreline->syncD == core_line.syncD &&
           (input_line.input_arena || input_line.midi_datas) &&
           judge->inits.inputline->input_arena == input_line.input_arena &&
           judge->inits.inputline->midi_datas == input_line.midi_datas;
}

void
FACADE::ActivateSuspendedSession()
{
    const auto player = core->player;
    if (!player || player->IsActive() || !judge->IsRunning() ||
        !judge->IsSuspended() ||
        input->GetState() != PDJE_INPUT_STATE::INPUT_LOOP_RUNNING ||
        !HasCurrentDataLines()) {
        warnlog("FACADE cannot activate an invalid suspended session");
        return;
    }
    try {
        auto *sync_line  = player->PullOutDataLine().syncD;
        auto  sync       = sync_line->load(std::memory_order_acquire);
        sync.microsecond = 0;
        sync_line->store(sync, std::memory_order_release);
        // The single Judge consumer sets a raw-time cutoff and discards batches
        // crossing this boundary. Until a fresh audio callback, it only drains.
        if (!judge->ResumeJudgments()) {
            critlog("FACADE failed to resume Judge; session remains suspended");
            return;
        }
        if (!player->Activate()) {
            critlog("FACADE failed to resume Core; ending the session");
            End();
            return;
        }
        state = STATE::PLAYING;
    } catch (const std::exception &e) {
        critlog("FACADE failed to activate suspended session: {}", e.what());
        End();
    } catch (...) {
        critlog(
            "FACADE failed to activate suspended session: unknown exception");
        End();
    }
}

void
FACADE::Ready(const CoreReady  &core_ready,
              const InputReady &input_ready,
              const JudgeReady &judge_ready)
{
    if (state == STATE::FAULTED) {
        warnlog("FACADE::Ready requires End after a failed control operation");
        return;
    }
    const auto player = core->player;
    if (!player || player->IsActive() || judge->IsRunning() ||
        input->GetState() != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) {
        throw std::logic_error(
            "FACADE::Ready requires an initialized, stopped player/Judge and "
            "Input in DEVICE_CONFIG_STATE");
    }
    state = STATE::STOPPED;
    if (input_ready.devices.empty() && input_ready.midi_devices.empty()) {
        throw std::invalid_argument("FACADE::Ready requires selected devices");
    }
    if (!judge_ready.events.used_event || !judge_ready.events.missed_event) {
        throw std::invalid_argument(
            "FACADE::Ready requires use/miss callbacks");
    }
    if (judge_ready.event_rule.use_range_microsecond >
        judge_ready.event_rule.miss_range_microsecond) {
        throw std::invalid_argument("FACADE::Ready requires the use window not "
                                    "to exceed the miss window");
    }

    // Stage chart/rules without changing the existing modules. In particular,
    // register rails before collecting notes: the collector filters by rail.
    PDJE_JUDGE::Judge_Init prepared;
    prepared.SetCoreLine(player->PullOutDataLine());
    if (!prepared.coreline) {
        throw std::logic_error(
            "FACADE::Ready requires a Core data line accepted by native Judge");
    }
    for (const auto &rail : judge_ready.keyboard_mouse_rails) {
        if (rail.device.Type == PDJE_Dev_Type::UNKNOWN) {
            throw std::invalid_argument(
                "FACADE::Ready has an unknown device rail");
        }
        prepared.SetRail(rail.device,
                         rail.device_key,
                         rail.offset_microsecond,
                         rail.rail_id);
    }
    for (const auto &rail : judge_ready.midi_rails) {
        if (rail.port_name.empty() || rail.ch > 15 || rail.pos > 127) {
            throw std::invalid_argument(
                "FACADE::Ready has an invalid MIDI rail");
        }
        prepared.SetRail(rail.port_name,
                         rail.rail_id,
                         rail.type,
                         rail.ch,
                         rail.pos,
                         rail.offset_microsecond);
    }
    if (judge_ready.keyboard_mouse_rails.empty() &&
        judge_ready.midi_rails.empty()) {
        throw std::invalid_argument("FACADE::Ready requires registered rails");
    }
    prepared.SetEventRule(judge_ready.event_rule);
    prepared.SetCustomEvents(judge_ready.events);
    // An empty, successfully decoded chart is valid (e.g. axis-only gameplay).
    prepared.note_objects.emplace();
    auto track =
        core_ready.track; // The native loader takes a mutable reference.
    OBJ_SETTER_CALLBACK collect = std::bind_front(
        &PDJE_JUDGE::Judge_Init::NoteObjectCollector, &prepared);
    try {
        if (!core->GetNoteObjects(track, collect)) {
            critlog("FACADE::Ready failed to load chart notes");
            return;
        }
    } catch (const std::exception &e) {
        critlog("FACADE::Ready failed to load chart notes: {}", e.what());
        return;
    } catch (...) {
        critlog("FACADE::Ready failed to load chart notes: unknown exception");
        return;
    }
    auto devices = input_ready.devices;

    // Config may invalidate Input on failure. Detach old Judge views first.
    // From this boundary onward a failure tears down, rather than restoring the
    // previous setup. No Run(), Start(), Activate() or sync wait belongs here.
    judge->End();
    try {
        if (!input->Config(devices,
                           input_ready.midi_devices,
                           judge_ready.events.midi_cc_lsb_on)) {
            critlog("FACADE::Ready failed to configure Input");
            End();
            return;
        }
        prepared.SetInputLine(input->PullOutDataLine());
        if (!prepared.inputline) {
            critlog("FACADE::Ready has no configured input buffer");
            End();
            return;
        }
        judge->inits = std::move(prepared);
        // Judge::Start owns sorting and iterator initialization. Ready never
        // traverses notes; a pre-play Restart resets cursors inside Judge.
        state = STATE::READY;
    } catch (const std::exception &e) {
        critlog("FACADE::Ready failed while configuring the session: {}",
                e.what());
        End();
    } catch (...) {
        critlog("FACADE::Ready failed while configuring the session: unknown "
                "exception");
        End();
    }
}

void
FACADE::Play()
{
    if (state == STATE::FAULTED || state == STATE::PAUSED) {
        warnlog(
            "FACADE::Play requires End after failure, or Resume when paused");
        return;
    }
    if (state == STATE::READY && judge->IsRunning() && judge->IsSuspended()) {
        // Restart keeps acquisition/polling alive while requiring explicit
        // Play.
        ActivateSuspendedSession();
        return;
    }
    // Lifecycle operations and external module changes must be serialized by
    // the caller. Reject invalid preparation before touching any running state.
    const auto player = core->player;
    if (!player) {
        throw std::logic_error("FACADE::Play requires an initialized player");
    }
    if (player->IsActive() || judge->IsRunning() ||
        input->GetState() != PDJE_INPUT_STATE::INPUT_LOOP_READY) {
        throw std::logic_error(
            "FACADE::Play requires stopped Core/Judge and configured Input");
    }

    const auto  core_line = player->PullOutDataLine();
    const auto &init      = judge->inits;
    if (!HasCurrentDataLines()) {
        throw std::logic_error(
            "FACADE::Play requires Judge data lines attached to these modules");
    }
    if (!init.note_objects || !init.ev_rule || !init.lambdas.used_event ||
        !init.lambdas.missed_event) {
        throw std::logic_error(
            "FACADE::Play requires notes, rails, rules and use/miss callbacks");
    }

    // The device is stopped. Invalidate only the old wall-clock anchor, not
    // the song cursor; Judge drains inputs until a new audio callback publishes
    // a valid sync sample. This is startup, not a seek/restart operation.
    auto sync        = core_line.syncD->load(std::memory_order_acquire);
    sync.microsecond = 0;
    core_line.syncD->store(sync, std::memory_order_release);

    try {
        if (!input->Run()) {
            critlog("FACADE::Play failed to start Input");
            End();
            return;
        }
        const auto status = judge->Start();
        if (status != PDJE_JUDGE::JUDGE_STATUS::OK) {
            critlog("FACADE::Play failed to start Judge; status={}",
                    static_cast<int>(status));
            End();
            return;
        }
        if (!player->Activate()) {
            critlog("FACADE::Play failed to start Core");
            End();
            return;
        }
        state = STATE::PLAYING;
    } catch (const std::exception &e) {
        critlog("FACADE::Play failed during startup: {}", e.what());
        End();
    } catch (...) {
        critlog("FACADE::Play failed during startup: unknown exception");
        End();
    }
}

void
FACADE::Pause()
{
    if (state == STATE::PAUSED) {
        return;
    }
    if (state != STATE::PLAYING || !core->player || !judge->IsRunning() ||
        input->GetState() != PDJE_INPUT_STATE::INPUT_LOOP_RUNNING ||
        !HasCurrentDataLines()) {
        warnlog("FACADE::Pause requires the current playing session");
        return;
    }
    try {
        // Wait for any in-flight judgment/axis callback, but not for blocked
        // IPC polling or already committed use/miss callbacks.
        if (!judge->SuspendJudgments()) {
            critlog("FACADE::Pause failed to suspend Judge");
            state = STATE::FAULTED;
            return;
        }
        if (!core->player->Deactivate()) {
            critlog(
                "FACADE::Pause failed to stop Core; Judge remains suspended");
            state = STATE::FAULTED;
            return;
        }
        state = STATE::PAUSED;
    } catch (const std::exception &e) {
        critlog("FACADE::Pause failed: {}", e.what());
        state = STATE::FAULTED;
    } catch (...) {
        critlog("FACADE::Pause failed: unknown exception");
        state = STATE::FAULTED;
    }
}

void
FACADE::Resume()
{
    if (state != STATE::PAUSED) {
        warnlog("FACADE::Resume requires PAUSED; use Play after Restart");
        return;
    }
    ActivateSuspendedSession();
}

void
FACADE::Restart()
{
    const auto player = core->player;
    if ((state != STATE::READY && state != STATE::PLAYING &&
         state != STATE::PAUSED) ||
        !player || !HasCurrentDataLines() || !judge->inits.note_objects) {
        warnlog(
            "FACADE::Restart requires a prepared, playing or paused session");
        return;
    }
    // Reject unsupported audio histories before altering playback or notes.
    if (!player->CanResetForRestart()) {
        warnlog("FACADE::Restart is unsupported by this Core configuration");
        return;
    }
    const bool has_runtime = judge->IsRunning();
    if (input->GetState() != (has_runtime
                                  ? PDJE_INPUT_STATE::INPUT_LOOP_RUNNING
                                  : PDJE_INPUT_STATE::INPUT_LOOP_READY)) {
        warnlog("FACADE::Restart has inconsistent Input/Judge state");
        return;
    }
    try {
        if (has_runtime && !judge->SuspendJudgments()) {
            critlog("FACADE::Restart failed to suspend Judge");
            state = STATE::FAULTED;
            return;
        }
        if (!player->Deactivate()) {
            critlog("FACADE::Restart failed to stop Core");
            state = STATE::FAULTED;
            return;
        }
        // This joins old callback workers, discards old queued jobs and rewinds
        // notes. The input poller remains the sole consumer, suspended.
        if (!judge->ResetForRestart() || !player->ResetForRestart()) {
            critlog("FACADE::Restart could not reset the session; call End");
            state = STATE::FAULTED;
            return;
        }
        state = STATE::READY;
    } catch (const std::exception &e) {
        critlog("FACADE::Restart failed: {}", e.what());
        state = STATE::FAULTED;
    } catch (...) {
        critlog("FACADE::Restart failed: unknown exception");
        state = STATE::FAULTED;
    }
}

void
FACADE::End()
{
    state        = STATE::FAULTED;
    bool stopped = true;
    // Joining Judge first prevents input-buffer invalidation and judging
    // against a stopped audio clock. If joining fails, keep the producers
    // alive.
    try {
        judge->End();
    } catch (const std::exception &e) {
        critlog("FACADE::End failed to join Judge; producers retained: {}",
                e.what());
        return;
    } catch (...) {
        critlog("FACADE::End failed to join Judge; producers retained: unknown "
                "exception");
        return;
    }

    try {
        if (core->player && !core->player->Deactivate()) {
            critlog("FACADE::End failed to stop Core; retry End before reuse");
            stopped = false;
        }
    } catch (const std::exception &e) {
        critlog("FACADE::End failed to stop Core: {}", e.what());
        stopped = false;
    } catch (...) {
        critlog("FACADE::End failed to stop Core: unknown exception");
        stopped = false;
    }

    // Still release Input when stopping audio fails. Judge is already joined.
    try {
        if (!input->Kill()) {
            critlog(
                "FACADE::End failed to release Input; retry End before reuse");
            stopped = false;
        }
    } catch (const std::exception &e) {
        critlog("FACADE::End failed to release Input: {}", e.what());
        stopped = false;
    } catch (...) {
        critlog("FACADE::End failed to release Input: unknown exception");
        stopped = false;
    }
    if (stopped) {
        state = STATE::STOPPED;
    }
}

} // namespace PDJE_GAMEPLAY
