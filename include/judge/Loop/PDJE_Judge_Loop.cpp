#include "PDJE_Judge_Loop.hpp"

#include "InputParser.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <chrono>
#include <cstddef>
#include <exception>

#include "PDJE_Benchmark.hpp"
#include <ratio>
#include <thread>
namespace PDJE_JUDGE {
Judge_Loop::Judge_Loop(Judge_Init &inits) : pre(&inits), match(&pre, &inits)
{
    init_datas = &inits;
}

bool
Judge_Loop::SuspendJudgments()
{
    // Waiting for this lock also quiesces direct mouse/MIDI callbacks and all
    // note mutation. Event workers intentionally keep draining committed jobs.
    std::lock_guard lock(production_mutex);
    suspended = true;
    ++generation;
    return true;
}

bool
Judge_Loop::IsSuspended() const
{
    std::lock_guard lock(production_mutex);
    return suspended;
}

bool
Judge_Loop::ResumeJudgments()
{
    {
        std::lock_guard lock(production_mutex);
        if (!suspended) {
            return true;
        }
    }
    // Lifecycle calls are serialized by the control thread. Keep production
    // gated until both workers exist. Never join workers holding the mutex.
    try {
        StartEventLoop();
    } catch (const std::exception &e) {
        critlog("cannot resume judgments: event worker startup failed");
        critlog(e.what());
        EndEventLoop();
        return false;
    } catch (...) {
        critlog(
            "cannot resume judgments: unknown event worker startup failure");
        EndEventLoop();
        return false;
    }
    std::lock_guard lock(production_mutex);
    // Use the producer clock, before calibration or song-local normalization.
    // The strict cutoff also drops events in the same clock tick as resume.
    raw_cutoff = clock_root.Get_MicroSecond();
    ++generation;
    suspended = false;
    return true;
}

bool
Judge_Loop::ResetForRestart()
{
    if (!init_datas->note_objects) {
        warnlog(
            "cannot reset judge loop for restart: notes are not configured");
        return false;
    }
    SuspendJudgments();
    EndEventLoop(); // All old callbacks finish before resetting note storage.
    std::lock_guard lock(production_mutex);
    // No producer or consumer can access either queue now. Get clears the
    // opposite side and flips buffers; clearing its result empties BOTH sides.
    pre.Event_Datas.use_queue.Get()->clear();
    pre.Event_Datas.miss_queue.Get()->clear();
    init_datas->note_objects->ResetForRestart();
    // Collection continues outside this lock. Do not touch parsed_res here:
    // generation rejects its batch and the next collection clears it.
    return true;
}

void
Judge_Loop::loop_once()
{
    WBCH("judge loop head")
    uint64_t batch_generation;
    uint64_t batch_cutoff;
    {
        std::lock_guard lock(production_mutex);
        batch_generation = generation;
        batch_cutoff     = raw_cutoff;
    }
    // Receive may block. Suspension must not wait for IPC or stop acquisition.
    const bool      has_inputs = pre.CollectInputs(batch_cutoff);
    std::lock_guard lock(production_mutex);
    if (suspended || batch_generation != generation) {
        return;
    }
    if (!pre.ProcessCollected(has_inputs)) {
        return;
    }
    if (init_datas->inputline->input_arena) {
        for (const PDJE_Input_Log &input_ev : pre.parsed_res.logs) {
            switch (input_ev.type) {
            case PDJE_Dev_Type::KEYBOARD:
                match.UseEvent<PDJE_Dev_Type::KEYBOARD>(input_ev);
                break;
            case PDJE_Dev_Type::MOUSE:
                match.UseEvent<PDJE_Dev_Type::MOUSE>(input_ev);
                break;
            default:
                break;
            }
        }
    }
    if (init_datas->inputline->midi_datas) {
        for (const auto &midi_ev : pre.parsed_res.midi_logs) {
            match.UseEvent(midi_ev);
        }
    }
    WBCH("judge loop tail")
}

void
Judge_Loop::StartEventLoop()
{
    if (!Event_Controls.use_event_thread) {
        Event_Controls.use_event_thread.emplace([this](std::stop_token token) {
            auto use_clock = std::chrono::steady_clock::now();
            WBCH("use event loop init")
            while (!token.stop_requested()) {
                try {
                    WBCH("use event line head")
                    use_clock += init_datas->lambdas.use_event_sleep_time;
                    std::this_thread::sleep_until(use_clock);

                    auto queue = pre.Event_Datas.use_queue.Get();
                    for (const auto &used : (*queue)) {
                        init_datas->lambdas.used_event(
                            used.railid, used.Pressed, used.IsLate, used.diff);
                    }
                    WBCH("use event line tail")
                } catch (const std::exception &e) {
                    critlog("caught error on use event loop. Why:");
                    critlog(e.what());
                }
            }
        });
    }
    if (!Event_Controls.miss_event_thread) {
        Event_Controls.miss_event_thread.emplace([this](std::stop_token token) {
            auto miss_clock = std::chrono::steady_clock::now();
            WBCH("miss event init")
            while (!token.stop_requested()) {
                try {
                    WBCH("miss event line head")
                    miss_clock += init_datas->lambdas.miss_event_sleep_time;
                    std::this_thread::sleep_until(miss_clock);
                    auto queue = pre.Event_Datas.miss_queue.Get();
                    for (const auto &missed : (*queue)) {
                        init_datas->lambdas.missed_event(missed);
                    }
                    WBCH("miss event line tail")
                } catch (const std::exception &e) {
                    critlog("caught error on miss event loop. Why:");
                    critlog(e.what());
                }
            }
        });
    }
}

void
Judge_Loop::EndEventLoop()
{
    if (Event_Controls.use_event_thread) {
        Event_Controls.use_event_thread->request_stop();
    }
    if (Event_Controls.miss_event_thread) {
        Event_Controls.miss_event_thread->request_stop();
    }
    Event_Controls.use_event_thread.reset();
    Event_Controls.miss_event_thread.reset();
}

}; // namespace PDJE_JUDGE
