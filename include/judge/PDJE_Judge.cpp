#include "PDJE_Judge.hpp"
#include "Input_State.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <atomic>
#include <cstdint>
#include <thread>
#include <unordered_map>
namespace PDJE_JUDGE {

void
JUDGE::SetCoreLine(const PDJE_CORE_DATA_LINE &coreline)
{
    if (coreline.maxCursor != nullptr && coreline.nowCursor != nullptr &&
        coreline.preRenderedData != nullptr && coreline.syncD != nullptr) {
        core = coreline;
    }
}

void
JUDGE::SetInputLine(const PDJE_INPUT_DATA_LINE &inputline)
{
    if (inputline.input_arena != nullptr && inputline.id_name_conv != nullptr) {
        input = inputline;
    }
}

void
JUDGE::NoteObjectCollector(const std::string        noteType,
                           const uint16_t           noteDetail,
                           const std::string        firstArg,
                           const std::string        secondArg,
                           const std::string        thirdArg,
                           const unsigned long long Y_Axis,
                           const unsigned long long Y_Axis_2,
                           const uint64_t           railID)
{
    if (!note_obj.has_value()) {
        note_obj.emplace();
    }
    PDJE_Dev_Type dtype;
    if (noteType == "KEYBOARD") {
        dtype = PDJE_Dev_Type::KEYBOARD;
    } else if (noteType == "MOUSE") {
        dtype = PDJE_Dev_Type::MOUSE;
    } else if (noteType == "HID") {
        dtype = PDJE_Dev_Type::HID;
    } else {
        dtype = PDJE_Dev_Type::UNKNOWN;
    }
    auto tempobj = NOTE{ .type   = dtype,
                         .detail = noteDetail,
                         .first  = firstArg,
                         .second = secondArg,
                         .third  = thirdArg,
                         .pos    = 0,
                         .used   = false };

    tempobj.pos = Y_Axis;
    switch (tempobj.type) {
    case PDJE_Dev_Type::KEYBOARD: {
        note_obj->Fill<KEY_IN>(tempobj, railID);
        if (Y_Axis_2 != 0) {
            tempobj.pos = Y_Axis_2;
            note_obj->Fill<KEY_OUT>(tempobj, railID);
        }
        break;
    }
    case PDJE_Dev_Type::MOUSE: {
        note_obj->Fill<AXIS_DEV_IN>(tempobj, railID);
        if (Y_Axis_2 != 0) {
            tempobj.pos = Y_Axis_2;
            note_obj->Fill<AXIS_DEV_OUT>(tempobj, railID);
        }
        break;
    }
    case PDJE_Dev_Type::HID: {
        note_obj->Fill<HID_DEV>(tempobj, railID);
        break;
    }
    default:
        break;
    }
}

void
JUDGE::SetInputRule(const INPUT_CONFIG &device_config)
{
    INPUT_RULE rule{ .Device_ID   = device_config.Device_ID,
                     .MatchType   = device_config.MatchType,
                     .MatchDetail = device_config.MatchDetail };
    dev_rules[rule] = device_config.MatchRail;
}

void
JUDGE::SetEventRule(const EVENT_RULE &event_rule)
{
    ev_rule = event_rule;
}

JUDGE_STATUS
JUDGE::Start()
{
    if (!core.has_value()) {
        return JUDGE_STATUS::CORE_LINE_IS_MISSING;
    }
    if (!input.has_value()) {
        return JUDGE_STATUS::INPUT_LINE_IS_MISSING;
    }
    if (!note_obj.has_value()) {
        return JUDGE_STATUS::NOTE_OBJECT_IS_MISSING;
    }
    if (!ev_rule.has_value()) {
        return JUDGE_STATUS::EVENT_RULE_IS_EMPTY;
    }
    if (dev_rules.empty()) {
        return JUDGE_STATUS::INPUT_RULE_IS_EMPTY;
    }
    note_obj->Sort();
    loop_switch = true;
    loop.emplace([this]() { Judge_Loop(); });
    return JUDGE_STATUS::OK;
}

JUDGE_STATUS
JUDGE::CheckStatus()
{
    return status;
}

void
JUDGE::End()
{
    loop_switch = false;
    loop->join();
    core.reset();
    input.reset();
    note_obj.reset();
}
constexpr double OneSample = 1000.0 / 48.0;
uint64_t
JUDGE::FrameToMicro(uint64_t frame,
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

void
JUDGE::Match(const PDJE_Input_Log &input,
             const P_NOTE_VEC     &note_list,
             bool                  isPressed)
{

    INPUT_RULE ir;
    for (const auto &note_local : note_list) {
        noteMicro = FrameToMicro(
            note_local->pos, sync_data.consumed_frames, sync_data.microsecond);
        isLate = noteMicro < input.microSecond;

        diff = isLate ? input.microSecond - noteMicro
                      : noteMicro - input.microSecond;
        if (diff < ev_rule->use_range_microsecond) {
            note_local->used = true;
            judge_event.used_event(ir, isPressed, isLate, diff);

            break;
        }
    }
}

void
JUDGE::Parse_Mouse(BITMASK ev, std::vector<int> &parsed_res)
{
    if (ev & PDJE_MOUSE_L_BTN_DOWN)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_L_IN);
    if (ev & PDJE_MOUSE_L_BTN_UP)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_L_OUT);
    if (ev & PDJE_MOUSE_R_BTN_DOWN)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_R_IN);
    if (ev & PDJE_MOUSE_R_BTN_UP)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_R_OUT);
    if (ev & PDJE_MOUSE_M_BTN_DOWN)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_M_IN);
    if (ev & PDJE_MOUSE_M_BTN_UP)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_M_OUT);
    if (ev & PDJE_MOUSE_SIDE_BTN_DOWN)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_SIDE_IN);
    if (ev & PDJE_MOUSE_SIDE_BTN_UP)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_SIDE_OUT);
    if (ev & PDJE_MOUSE_EX_BTN_DOWN)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_EX_IN);
    if (ev & PDJE_MOUSE_EX_BTN_UP)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::BTN_EX_OUT);
    if (ev & PDJE_MOUSE_XWHEEL)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::WHEEL_X);
    if (ev & PDJE_MOUSE_YWHEEL)
        parsed_res.push_back(DEVICE_MOUSE_EVENT::WHEEL_Y);
}

void
JUDGE::Judge_Loop()
{
    bool TypeOK = true;
    I_stat      = std::vector<int>(13);
    while (loop_switch) {
        // init cut time
        input_log = input->input_arena->Get();
        if (input_log->empty()) {
            continue;
        }
        log_begin_time = input_log->front().microSecond;
        cut_time       = log_begin_time > ev_rule->miss_range_microsecond
                             ? log_begin_time - ev_rule->miss_range_microsecond
                             : 0;
        // cut
        note_obj->Cut<KEY_IN>(cut_time, missed_in);
        note_obj->Cut<KEY_OUT>(cut_time, missed_out);
        note_obj->Cut<AXIS_DEV_IN>(cut_time, missed_axis_in);
        note_obj->Cut<AXIS_DEV_OUT>(cut_time, missed_axis_out);
        note_obj->Cut<HID_DEV>(cut_time, missed_hid);

        judge_event.missed_event(missed_in, missed_out);

        // init maximum get time
        log_end_time = input_log->back().microSecond;
        use_time     = log_end_time + ev_rule->use_range_microsecond;

        sync_data = core->syncD->load(std::memory_order_acquire);
        // sync with core engine
        TypeOK = true;
        for (const auto &input_ev : *input_log) {
            I_stat.clear();
            ir.Device_ID = input_ev.id;
            switch (input_ev.type) {
            case PDJE_Dev_Type::KEYBOARD: {
                ir.MatchDetail = input_ev.event.keyboard.k;
                I_stat.push_back(input_ev.event.keyboard.pressed ? 1 : 0);

                break;
            }
            case PDJE_Dev_Type::MOUSE: {

                ir.MatchDetail = noimplcrash;
                Parse_Mouse(input_ev.event.mouse.button_type, I_stat);
                break;
            }
            case PDJE_Dev_Type::HID: {
                ir.MatchDetail = 0;
                break;
            }
            default:
                TypeOK = false;
                break;
            }
            if (!TypeOK) {
                continue;
            }
            ir.MatchType = input_ev.type;

            if (auto device_itr = dev_rules.find(ir);
                device_itr != dev_rules.end()) {
                railID = device_itr->second;
            } else {
                continue;
            }
            switch (input_ev.type) {
            case PDJE_Dev_Type::KEYBOARD:
                if (I_stat.front() == 1) {
                    note_obj->Get<IN>(use_time, railID, related_list_in);
                    Match(input_ev, related_list_in, true);
                } else {
                    note_obj->Get<IN>(use_time, railID, related_list_out);
                    Match(input_ev, related_list_out, false);
                }
                break;
            case PDJE_Dev_Type::MOUSE:
                for (const auto &i : I_stat) {
                    // todo - impl
                }
            default:
                break;
            }
        }
    }
}

}; // namespace PDJE_JUDGE