#include "PDJE_Judge_Loop.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>
namespace PDJE_JUDGE {
Judge_Loop::Judge_Loop(Judge_Init &inits)
{
    init_datas = &inits;
    Cached.mouse_btn_event_queue.reserve(7);
}
bool
Judge_Loop::FindRailID(const INPUT_RULE &rule, uint64_t &id)
{
    std::cout << "dev rule size: " << init_datas->dev_rules.size() << std::endl;
    if (auto device_itr = init_datas->dev_rules.find(rule);
        device_itr != init_datas->dev_rules.end()) {
        id = device_itr->second;
        return true;
    } else {
        return false;
    }
}

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
        if (Cached.diff < init_datas->ev_rule->use_range_microsecond) {
            note_local->used = true;
            Event_Datas.use_queue.Write(
                { Cached.railID, isPressed, Cached.isLate, Cached.diff });

            break;
        }
    }
}

void
Judge_Loop::Cut()
{
    Cached.missed_buffers.clear();
    init_datas->note_objects->Cut<BUFFER_MAIN>(Cached.cut_range,
                                               Cached.missed_buffers);
                                               
    if (!Cached.missed_buffers.empty()) {
        Event_Datas.miss_queue.Write(Cached.missed_buffers);
    }
    init_datas->note_objects->Cut<BUFFER_SUB>(Cached.cut_range,
                                              Cached.missed_buffers);
    if (!Cached.missed_buffers.empty()) {
    
        Event_Datas.miss_queue.Write(Cached.missed_buffers);
    }
}

bool
Judge_Loop::PreProcess()
{
    input_log = init_datas->inputline->input_arena->Get();
    Cached.synced_data =
        init_datas->coreline->syncD->load(std::memory_order_acquire);

    Cached.local_microsecond_position =
        Convert_Frame_Into_MicroSecond(Cached.synced_data.consumed_frames);
    Cached.global_local_diff =
        Cached.synced_data.microsecond - Cached.local_microsecond_position;

    if (input_log->empty()) {
        
        Cached.cut_range = Cached.local_microsecond_position < init_datas->ev_rule->miss_range_microsecond ? 0 :
        Cached.local_microsecond_position -
                           init_datas->ev_rule->miss_range_microsecond;
        Cut();
        return false;
    }
    
    Cached.log_begin =
        input_log->front().microSecond < Cached.global_local_diff ?
        0
        :
        input_log->front().microSecond - Cached.global_local_diff;
    Cached.cut_range =
        Cached.log_begin < init_datas->ev_rule->miss_range_microsecond ?
        0
        :
        Cached.log_begin - init_datas->ev_rule->miss_range_microsecond;

    Cut();

    // init maximum get time
    Cached.log_end = input_log->back().microSecond - Cached.global_local_diff;
    Cached.use_range =
        Cached.log_end + init_datas->ev_rule->use_range_microsecond;
    return true;
}
void
Judge_Loop::loop()
{
    bool OK = true;
    while (loop_switch) {
        OK = PreProcess();
        if (!OK) {
            continue;
        }
        for (const auto &input_ev : *input_log) {
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
    }
}
void
Judge_Loop::StartEventLoop()
{
    Event_Controls.use_event_switch = true;
    Event_Controls.use_event_thread.emplace([this]() {
        auto use_clock = std::chrono::steady_clock::now();
        while (Event_Controls.use_event_switch.value()) {
            use_clock += init_datas->lambdas.use_event_sleep_time;
            std::this_thread::sleep_until(use_clock);

            auto queue = Event_Datas.use_queue.Get();

            for (const auto &log : *queue) {
                init_datas->lambdas.used_event(
                    log.railid, log.Pressed, log.IsLate, log.diff);
            }
        }
    });
    Event_Controls.miss_event_switch = true;
    Event_Controls.miss_event_thread.emplace([this]() {
        auto miss_clock = std::chrono::steady_clock::now();
        while (Event_Controls.miss_event_switch.value()) {
            miss_clock += init_datas->lambdas.miss_event_sleep_time;
            std::this_thread::sleep_for(
                init_datas->lambdas.miss_event_sleep_time);
            auto queue = Event_Datas.miss_queue.Get();
            for (const auto &log : *queue) {
                init_datas->lambdas.missed_event(log);
            }
        }
    });
}

void
Judge_Loop::EndEventLoop()
{
    Event_Controls.use_event_switch  = false;
    Event_Controls.miss_event_switch = false;
    Event_Controls.use_event_thread->join();
    Event_Controls.miss_event_thread->join();
}

}; // namespace PDJE_JUDGE