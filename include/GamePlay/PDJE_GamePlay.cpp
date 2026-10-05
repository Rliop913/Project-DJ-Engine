#include "PDJE_GamePlay.hpp"
#include "PDJE_Input.hpp"
#include "PDJE_Judge.hpp"
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

void
FACADE::Ready(const CoreReady  &core_ready,
              const InputReady &input_ready,
              const JudgeReady &judge_ready)
{
    const auto player = core->player;
    if (!player || player->IsActive() || judge->IsRunning() ||
        input->GetState() != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) {
        throw std::logic_error(
            "FACADE::Ready requires an initialized, stopped player/Judge and "
            "Input in DEVICE_CONFIG_STATE");
    }
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
    if (prepared.raildb.Empty()) {
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
    if (!core->GetNoteObjects(track, collect)) {
        throw std::runtime_error("FACADE::Ready failed to load chart notes");
    }
    // Initialize iterators before transferring the staged note object.
    prepared.note_objects->Sort();
    auto devices = input_ready.devices;

    // Config may invalidate Input on failure. Detach old Judge views first.
    // From this boundary onward a failure tears down, rather than restoring the
    // previous setup. No Run(), Start(), Activate() or sync wait belongs here.
    judge->End();
    try {
        if (!input->Config(devices,
                           input_ready.midi_devices,
                           judge_ready.events.midi_cc_lsb_on)) {
            throw std::runtime_error("FACADE::Ready failed to configure Input");
        }
        prepared.SetInputLine(input->PullOutDataLine());
        if (!prepared.inputline) {
            throw std::runtime_error(
                "FACADE::Ready has no configured input buffer");
        }
        judge->inits = std::move(prepared);
        // OBJ currently copies on move; rebuild NOTE_ITR cursors so none refer
        // back to the temporary's vectors. Play will sort again at
        // Judge::Start.
        judge->inits.note_objects->Sort();
    } catch (...) {
        const auto ready_error = std::current_exception();
        try {
            End();
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                "FACADE::Ready failed and cleanup was incomplete; retry End() "
                "before reinitializing"));
        }
        std::rethrow_exception(ready_error);
    }
}

void
FACADE::Play()
{
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

    const auto  core_line  = player->PullOutDataLine();
    const auto  input_line = input->PullOutDataLine();
    const auto &init       = judge->inits;
    if (!core_line.syncD || !init.coreline ||
        init.coreline->syncD != core_line.syncD || !init.inputline ||
        (!input_line.input_arena && !input_line.midi_datas) ||
        init.inputline->input_arena != input_line.input_arena ||
        init.inputline->midi_datas != input_line.midi_datas) {
        throw std::logic_error(
            "FACADE::Play requires Judge data lines attached to these modules");
    }
    if (!init.note_objects || !init.ev_rule || judge->inits.raildb.Empty() ||
        !init.lambdas.used_event || !init.lambdas.missed_event) {
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
            throw std::runtime_error("FACADE::Play failed to start Input");
        }
        const auto status = judge->Start();
        if (status != PDJE_JUDGE::JUDGE_STATUS::OK) {
            throw std::runtime_error(
                "FACADE::Play failed to start Judge; status=" +
                std::to_string(static_cast<int>(status)));
        }
        if (!player->Activate()) {
            throw std::runtime_error("FACADE::Play failed to start Core");
        }
    } catch (...) {
        const auto start_error = std::current_exception();
        try {
            End();
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                "FACADE::Play failed and cleanup was incomplete; retry End() "
                "before reinitializing"));
        }
        std::rethrow_exception(start_error);
    }
}

void
FACADE::Pause()
{
}

void
FACADE::Resume()
{
}

void
FACADE::Restart()
{
}

void
FACADE::End()
{
    // Joining Judge first prevents input-buffer invalidation and judging
    // against a stopped audio clock. If joining fails, keep the producers
    // alive.
    judge->End();

    std::exception_ptr error;
    try {
        if (core->player && !core->player->Deactivate()) {
            throw std::runtime_error("FACADE::End failed to stop Core");
        }
    } catch (...) {
        error = std::current_exception();
    }

    // Still release Input when stopping audio fails. Judge is already joined.
    try {
        if (!input->Kill()) {
            throw std::runtime_error("FACADE::End failed to release Input");
        }
    } catch (...) {
        if (!error) {
            error = std::current_exception();
        }
    }
    if (error) {
        std::rethrow_exception(error);
    }
}

} // namespace PDJE_GAMEPLAY
