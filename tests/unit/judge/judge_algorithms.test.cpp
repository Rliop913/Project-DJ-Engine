#include "Init/PDJE_Judge_Init_Structs.hpp"
#include "PDJE_PreProcess.hpp"
#include "PDJE_RAIL.hpp"
#include <doctest/doctest.h>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace {

constexpr uint64_t kFrameRate           = 48000;
constexpr uint64_t kTestRail            = 5;
constexpr uint64_t kMissRangeMicrosecond = 100000;
constexpr uint64_t kUseRangeMicrosecond  = 100000;

struct JudgePreprocessHarness {
    uint64_t now_cursor  = 0;
    uint64_t max_cursor  = kFrameRate * 120;
    float    prerendered = 0.0F;

    std::atomic<audioSyncData> sync;
    Atomic_Double_Buffer<PDJE_MIDI::MIDI_EV> midi_buffer { 16 };
    PDJE_JUDGE::Judge_Init init;

    JudgePreprocessHarness()
    {
        sync.store(audioSyncData {}, std::memory_order_release);

        PDJE_CORE_DATA_LINE core_line {};
        core_line.nowCursor       = &now_cursor;
        core_line.maxCursor       = &max_cursor;
        core_line.preRenderedData = &prerendered;
        core_line.syncD           = &sync;
        init.coreline             = core_line;

        PDJE_INPUT_DATA_LINE input_line {};
        input_line.midi_datas = &midi_buffer;
        init.inputline        = input_line;

        PDJE_JUDGE::EVENT_RULE event_rule {};
        event_rule.miss_range_microsecond = kMissRangeMicrosecond;
        event_rule.use_range_microsecond  = kUseRangeMicrosecond;
        init.ev_rule                      = event_rule;

        init.note_objects.emplace();
    }

    void
    SetSync(const uint64_t consumed_frames,
            const uint64_t pre_calculated_unused_frames,
            const uint64_t microsecond)
    {
        audioSyncData data {};
        data.consumed_frames                = consumed_frames;
        data.pre_calculated_unused_frames   = pre_calculated_unused_frames;
        data.microsecond                    = microsecond;
        sync.store(data, std::memory_order_release);
    }

    void
    AddMainNote(const int64_t microsecond)
    {
        PDJE_JUDGE::NOTE note;
        note.type        = "tap";
        note.microsecond = microsecond;
        init.note_objects->Fill<PDJE_JUDGE::BUFFER_MAIN>(note, kTestRail);
        init.note_objects->Sort();
    }

    void
    WriteMidiEvent(const uint64_t highres_time)
    {
        PDJE_MIDI::MIDI_EV event {};
        event.highres_time = highres_time;
        midi_buffer.Write(event);
    }
};

std::size_t
CountMisses(
    const std::vector<std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC>>
        &batches)
{
    std::size_t total = 0;
    for (const auto &batch : batches) {
        for (const auto &rail_notes : batch) {
            total += rail_notes.second.size();
        }
    }
    return total;
}

} // namespace

TEST_CASE("judge: frame to microsecond conversion matches 48kHz basis")
{
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(0) == 0);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(48) == 1000);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(96) == 2000);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(6) == 125);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(12) == 250);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(48000) == 1000000);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(96000) == 2000000);
}

TEST_CASE("judge: frame to microsecond conversion uses deterministic floor policy")
{
    // Regression guard: avoid floating-point rounding drift on exact boundaries.
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(1) == 20);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(2) == 41);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(3) == 62);

    uint64_t prev = PDJE_JUDGE::Convert_Frame_Into_MicroSecond(0);
    for (uint64_t frame = 1; frame <= 64; ++frame) {
        const auto cur = PDJE_JUDGE::Convert_Frame_Into_MicroSecond(frame);
        CHECK(cur >= prev);
        prev = cur;
    }

    for (uint64_t frame = 0; frame <= 64; ++frame) {
        CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(frame + 6) ==
              PDJE_JUDGE::Convert_Frame_Into_MicroSecond(frame) + 125);
    }
}

TEST_CASE("judge: KB_MOUSE rail key works with unordered_map hashing")
{
    PDJE_JUDGE::RAIL_KEY::KB_MOUSE k1;
    k1.Device_Name = "kbd-main";
    k1.DeviceKey   = static_cast<BITMASK>(PDJE_KEY::A);

    PDJE_JUDGE::RAIL_KEY::KB_MOUSE k2 = k1;

    std::unordered_map<PDJE_JUDGE::RAIL_KEY::KB_MOUSE, int> m;
    m[k1] = 42;

    REQUIRE(m.find(k2) != m.end());
    CHECK(m[k2] == 42);
}

TEST_CASE("judge: MIDI rail key equality and hashing are deterministic")
{
    PDJE_JUDGE::RAIL_KEY::MIDI m1;
    m1.port_name = "midi-port";
    m1.type      = 1;
    m1.ch        = 2;
    m1.pos       = 64;

    PDJE_JUDGE::RAIL_KEY::MIDI m2 = m1;

    CHECK(m1 == m2);
    CHECK(std::hash<PDJE_JUDGE::RAIL_KEY::MIDI>{}(m1) ==
          std::hash<PDJE_JUDGE::RAIL_KEY::MIDI>{}(m2));
}

TEST_CASE("judge: startup without audio sync drains input without misses")
{
    JudgePreprocessHarness harness;
    harness.SetSync(0, 0, 0);
    harness.AddMainNote(1000000);
    harness.WriteMidiEvent(50000000);

    PDJE_JUDGE::PreProcess pre(&harness.init);

    CHECK_FALSE(pre.Work());
    CHECK(pre.parsed_res.midi_logs.empty());
    CHECK(CountMisses(*pre.Event_Datas.miss_queue.Get()) == 0);
}

TEST_CASE("judge: local position uses atomic sync frame snapshot")
{
    JudgePreprocessHarness harness;
    harness.now_cursor = 0;
    harness.SetSync(kFrameRate, 0, 100000000);
    harness.AddMainNote(2000000);

    PDJE_JUDGE::PreProcess pre(&harness.init);

    CHECK_FALSE(pre.Work());
    CHECK(pre.local_micro_pos == 1000000);
    CHECK(CountMisses(*pre.Event_Datas.miss_queue.Get()) == 0);
}

TEST_CASE("judge: valid sync still misses genuinely expired notes")
{
    JudgePreprocessHarness harness;
    harness.now_cursor = kFrameRate;
    harness.SetSync(kFrameRate, 0, 10000000);
    harness.AddMainNote(500000);

    PDJE_JUDGE::PreProcess pre(&harness.init);

    CHECK_FALSE(pre.Work());
    CHECK(pre.local_micro_pos == 1000000);
    CHECK(CountMisses(*pre.Event_Datas.miss_queue.Get()) == 1);
}

TEST_CASE("judge: input before local zero is dropped instead of clamped")
{
    JudgePreprocessHarness harness;
    harness.now_cursor = kFrameRate;
    harness.SetSync(kFrameRate, 0, 10000000);
    harness.AddMainNote(2000000);
    harness.WriteMidiEvent(8999999);

    PDJE_JUDGE::PreProcess pre(&harness.init);

    CHECK_FALSE(pre.Work());
    CHECK(pre.parsed_res.midi_logs.empty());
    CHECK(CountMisses(*pre.Event_Datas.miss_queue.Get()) == 0);
}
