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
Judge_Loop::Judge_Loop(Judge_Init &inits)
{
    init_datas = &inits;
    Cached.mouse_btn_event_queue.reserve(7);
}
// bool
// Judge_Loop::FindDevSetting(const INPUT_RULE &rule, INPUT_SETTING &setting)
// {
//     if (auto device_itr = init_datas->dev_rules.find(rule);
//         device_itr != init_datas->dev_rules.end()) {
//         setting = device_itr->second;

//         return true;
//     } else {
//         return false;
//     }
// }

void
Judge_Loop::Match(const LOCAL_TIME  input_time,
                  const P_NOTE_VEC &note_list,
                  const uint64_t    railid,
                  const bool        isPressed)
{
    for (const auto &note_local : note_list) {

        Cached.isLate = note_local->microsecond < input_time;

        Cached.diff = Cached.isLate ? input_time - note_local->microsecond
                                    : note_local->microsecond - input_time;
        // std::cout << "Matching... " << Cached.diff << std::endl;
        if (Cached.diff <= init_datas->ev_rule->use_range_microsecond) {
            note_local->used = true;

            // std::cout << "Matched. and used. " << Cached.diff << std::endl;
            Event_Datas.use_queue.Write(
                { railid, isPressed, Cached.isLate, Cached.diff });

            break;
        }
    }
}

void
Judge_Loop::Cut(const uint64_t cut_range)
{
    Cached.missed_buffers.clear();
    init_datas->note_objects->Cut<BUFFER_MAIN>(cut_range,
                                               Cached.missed_buffers);

    if (!Cached.missed_buffers.empty()) {
        Event_Datas.miss_queue.Write(Cached.missed_buffers);
    }
    Cached.missed_buffers.clear();
    init_datas->note_objects->Cut<BUFFER_SUB>(cut_range, Cached.missed_buffers);
    if (!Cached.missed_buffers.empty()) {

        Event_Datas.miss_queue.Write(Cached.missed_buffers);
    }
}

PARSE_OUT *
Judge_Loop::PreProcess()
{

    input_log = init_datas->inputline->input_arena->Get();
    auto res  = init_datas->devparser.Parse(input_log);

    Cached.synced_data =
        init_datas->coreline->syncD->load(std::memory_order_acquire);

    Cached.local_microsecond_position =
        Convert_Frame_Into_MicroSecond(Cached.synced_data.consumed_frames);
    Cached.global_local_diff =
        Cached.synced_data.microsecond - Cached.local_microsecond_position;

    if (!res) {
        if (Cached.local_microsecond_position <
            init_datas->ev_rule->miss_range_microsecond) {
            Cut(0);
        } else {
            Cut(Cached.local_microsecond_position -
                init_datas->ev_rule->miss_range_microsecond);
        }
        return nullptr;
    }

    if (res->lowest < Cached.global_local_diff) {
        res->lowest = 0;
    } else {
        res->lowest -= Cached.global_local_diff;
    }
    if (res->lowest < init_datas->ev_rule->miss_range_microsecond) {
        Cut(0);
    } else {
        Cut(res->lowest - init_datas->ev_rule->miss_range_microsecond);
    }

    // init maximum get time
    if (res->highest < Cached.global_local_diff) {
        res->highest = 0;
    } else {
        res->highest -= Cached.global_local_diff;
    }
    Cached.use_range =
        res->highest + init_datas->ev_rule->use_range_microsecond;
    for (auto &log : res->logs) {
        if (log.microSecond < Cached.global_local_diff) {
            log.microSecond = 0;
        } else {
            log.microSecond -= Cached.global_local_diff;
        }
    }
    return res;
}
void
Judge_Loop::loop()
{
    WBCH("judge loop started")
    while (loop_switch) {
        WBCH("judge loop head")
        PARSE_OUT *res = PreProcess();
        if (!res) {
            continue;
        }

        for (const PDJE_Input_Log &input_ev : res->logs) {
            switch (input_ev.type) {
            case PDJE_Dev_Type::KEYBOARD:
                UseEvent<PDJE_Dev_Type::KEYBOARD>(input_ev);
                break;
            case PDJE_Dev_Type::MOUSE:
                UseEvent<PDJE_Dev_Type::MOUSE>(input_ev);
                break;
            case PDJE_Dev_Type::MIDI:
                UseEvent<PDJE_Dev_Type::MIDI>(input_ev);
                break;
            case PDJE_Dev_Type::HID:
                UseEvent<PDJE_Dev_Type::HID>(input_ev);
                break;
            default:
                break;
            }
        }
        WBCH("judge loop tail")
    }
}
void
Judge_Loop::StartEventLoop()
{
    Event_Controls.use_event_switch = true;
    Event_Controls.use_event_thread.emplace([this]() {
        auto use_clock = std::chrono::steady_clock::now();
        WBCH("use event loop init")
        while (Event_Controls.use_event_switch.value()) {
            try {
                WBCH("use event line head")
                use_clock += init_datas->lambdas.use_event_sleep_time;
                std::this_thread::sleep_until(use_clock);

                auto queue = Event_Datas.use_queue.Get();
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
    Event_Controls.miss_event_switch = true;
    Event_Controls.miss_event_thread.emplace([this]() {
        auto miss_clock = std::chrono::steady_clock::now();
        WBCH("miss event init")
        while (Event_Controls.miss_event_switch.value()) {
            try {
                WBCH("miss event line head")
                miss_clock += init_datas->lambdas.miss_event_sleep_time;
                std::this_thread::sleep_for(
                    init_datas->lambdas.miss_event_sleep_time);
                auto queue = Event_Datas.miss_queue.Get();
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

void
Judge_Loop::EndEventLoop()
{
    Event_Controls.use_event_switch  = false;
    Event_Controls.miss_event_switch = false;
    if (Event_Controls.use_event_thread->joinable()) {
        Event_Controls.use_event_thread->join();
    }
    if (Event_Controls.miss_event_thread->joinable()) {
        Event_Controls.miss_event_thread->join();
    }
}

}; // namespace PDJE_JUDGE