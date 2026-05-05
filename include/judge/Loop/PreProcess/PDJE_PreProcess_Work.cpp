#include "PDJE_PreProcess.hpp"

namespace PDJE_JUDGE {
namespace {

uint64_t
SaturatingSub(const uint64_t lhs, const uint64_t rhs)
{
    return lhs < rhs ? 0 : lhs - rhs;
}

uint64_t
PlaybackFrameFromSync(const audioSyncData &sync)
{
    return sync.consumed_frames;
}

bool
AudioSyncReady(const audioSyncData &sync)
{
    return sync.microsecond != 0;
}

void
ClearParsed(PARSE_OUT &parsed)
{
    parsed.logs.clear();
    parsed.midi_logs.clear();
    parsed.lowest  = 0;
    parsed.highest = 0;
}

void
ObserveTime(uint64_t &lowest,
            uint64_t &highest,
            bool     &has_time,
            uint64_t  local_time)
{
    if (!has_time) {
        lowest   = local_time;
        highest  = local_time;
        has_time = true;
        return;
    }

    if (local_time < lowest) {
        lowest = local_time;
    }
    if (local_time > highest) {
        highest = local_time;
    }
}

bool
NormalizeParsedInput(PARSE_OUT &parsed, const uint64_t global_local_diff)
{
    uint64_t lowest   = 0;
    uint64_t highest  = 0;
    bool     has_time = false;

    auto log_out = parsed.logs.begin();
    for (auto log : parsed.logs) {
        if (log.microSecond < global_local_diff) {
            continue;
        }
        log.microSecond -= global_local_diff;
        ObserveTime(lowest, highest, has_time, log.microSecond);
        *log_out = log;
        ++log_out;
    }
    parsed.logs.erase(log_out, parsed.logs.end());

    auto midi_out = parsed.midi_logs.begin();
    for (auto midi_log : parsed.midi_logs) {
        if (midi_log.highres_time < global_local_diff) {
            continue;
        }
        midi_log.highres_time -= global_local_diff;
        ObserveTime(lowest, highest, has_time, midi_log.highres_time);
        *midi_out = midi_log;
        ++midi_out;
    }
    parsed.midi_logs.erase(midi_out, parsed.midi_logs.end());

    if (!has_time) {
        ClearParsed(parsed);
        return false;
    }

    parsed.lowest  = lowest;
    parsed.highest = highest;
    return true;
}

void
ResetTimingWindow(PreProcess &pre)
{
    ClearParsed(pre.parsed_res);
    pre.local_micro_pos   = 0;
    pre.global_local_diff = 0;
    pre.use_range         = 0;
}

void
SetLocalTiming(PreProcess &pre)
{
    const auto real_frame =
        SaturatingSub(PlaybackFrameFromSync(pre.synced_data),
                      pre.synced_data.pre_calculated_unused_frames);
    pre.local_micro_pos = Convert_Frame_Into_MicroSecond(real_frame);
    pre.global_local_diff =
        SaturatingSub(pre.synced_data.microsecond, pre.local_micro_pos);
}

void
CutExpiredByLimit(PreProcess &pre,
                  const uint64_t current_limit,
                  const uint64_t miss_range)
{
    pre.Cut(current_limit < miss_range ? 0 : current_limit - miss_range);
}

void
CutExpiredByPlaybackPosition(PreProcess &pre, const EVENT_RULE &rule)
{
    CutExpiredByLimit(pre, pre.local_micro_pos, rule.miss_range_microsecond);
}

void
CutExpiredByInputPosition(PreProcess &pre, const EVENT_RULE &rule)
{
    CutExpiredByLimit(pre, pre.parsed_res.lowest, rule.miss_range_microsecond);
}

void
SetUseWindow(PreProcess &pre, const EVENT_RULE &rule)
{
    pre.use_range = pre.parsed_res.highest + rule.use_range_microsecond;
}

} // namespace

bool
PreProcess::Work()
{
    synced_data = init->coreline->syncD->load(std::memory_order_acquire);

    if (!AudioSyncReady(synced_data)) {
        GetDatas();
        ResetTimingWindow(*this);
        return false;
    }

    SetLocalTiming(*this);

    if (!GetDatas()) { // no input datas
        CutExpiredByPlaybackPosition(*this, *init->ev_rule);
        return false;
    }

    if (!NormalizeParsedInput(parsed_res, global_local_diff)) {
        CutExpiredByPlaybackPosition(*this, *init->ev_rule);
        return false;
    }

    CutExpiredByInputPosition(*this, *init->ev_rule);
    SetUseWindow(*this, *init->ev_rule);
    return true;
}
}; // namespace PDJE_JUDGE
