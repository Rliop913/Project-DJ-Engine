#include "PDJE_Judge_Loop.hpp"

namespace PDJE_JUDGE {
Judge_Loop::Judge_Loop(Judge_Init &inits)
{
    init_datas = &inits;
    temp_rules.reserve(7);
}
bool
Judge_Loop::FindRailID(const INPUT_RULE &rule, uint64_t &id)
{
    if (auto device_itr = init_datas->dev_rules.find(rule);
        device_itr != init_datas->dev_rules.end()) {
        id = device_itr->second;
        return true;
    } else {
        return false;
    }
}

void
Judge_Loop::Match(const uint64_t    input_time,
                  const P_NOTE_VEC &note_list,
                  const uint64_t    railid,
                  const bool        isPressed)
{
    for (const auto &note_local : note_list) {
        noteMicro = FrameToMicro(note_local->pos,
                                 synced_data.consumed_frames,
                                 synced_data.microsecond);
        isLate    = noteMicro < input_time;

        diff = isLate ? input_time - noteMicro : noteMicro - input_time;
        if (diff < init_datas->ev_rule->use_range_microsecond) {
            note_local->used = true;
            if (init_datas->use.has_value()) {
                init_datas->usedDatas =
                    Judge_Init::usedStruct{ .railid  = railid,
                                            .Pressed = isPressed,
                                            .IsLate  = isLate,
                                            .diff    = diff };
                init_datas->use->signal();
            }

            break;
        }
    }
}
constexpr double OneSample = 1000.0 / 48.0;
uint64_t
Judge_Loop::FrameToMicro(uint64_t frame,
                         uint64_t origin_frame,
                         uint64_t origin_microsecond)
{
    if (origin_frame > frame) {
        auto diff = origin_frame - frame;
        diff *= OneSample;
        return origin_microsecond - diff;
    } else {
        auto diff = frame - origin_frame;
        diff *= OneSample;
        return origin_microsecond + diff;
    }
}
bool
Judge_Loop::PreProcess()
{
    input_log = init_datas->inputline->input_arena->Get();
    if (input_log->empty()) {
        return false;
    }

    log_begin_time = input_log->front().microSecond;
    cut_range_time =
        log_begin_time > init_datas->ev_rule->miss_range_microsecond
            ? log_begin_time - init_datas->ev_rule->miss_range_microsecond
            : 0;
    // cut
    init_datas->note_objects->Cut<BUFFER_MAIN>(cut_range_time, missed_buffers);
    init_datas->note_objects->Cut<BUFFER_SUB>(cut_range_time, missed_buffers);
    // call miss event
    if (init_datas->miss.has_value()) {
        init_datas->missDatas = missed_buffers;
        init_datas->miss->signal();
    }

    // init maximum get time
    log_end_time   = input_log->back().microSecond;
    use_range_time = log_end_time + init_datas->ev_rule->use_range_microsecond;

    synced_data = init_datas->coreline->syncD->load(std::memory_order_acquire);
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

}; // namespace PDJE_JUDGE