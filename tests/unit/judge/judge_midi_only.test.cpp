#include "../input/dummy_midi_backend.hpp"
#include "PDJE_Judge.hpp"
#include <doctest/doctest.h>

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <future>
#include <limits>
#include <memory>
#include <vector>

using namespace PDJE_JUDGE;

namespace {

struct CoreFixture {
    unsigned long long         now_cursor  = 0;
    unsigned long long         max_cursor  = 48000;
    float                      prerendered = 0;
    std::atomic<audioSyncData> sync{ audioSyncData{ .microsecond = 1000000 } };

    void
    Attach(Judge_Init &init)
    {
        init.SetCoreLine({ &now_cursor, &max_cursor, &prerendered, &sync });
    }

    void
    Configure(Judge_Init                                 &init,
              Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> &buffer)
    {
        Attach(init);
        PDJE_INPUT_DATA_LINE line{};
        line.midi_datas = &buffer;
        init.SetInputLine(line);
        EVENT_RULE rule{};
        rule.miss_range_microsecond = 100000;
        rule.use_range_microsecond  = 100000;
        init.SetEventRule(rule);
        init.note_objects.emplace();
        init.SetRail("dummy-midi",
                     42,
                     static_cast<uint8_t>(libremidi::message_type::AFTERTOUCH),
                     0);
    }
};

struct AxisSample {
    LOCAL_TIME time;
    uint64_t   rail;
    double     value;
};

PDJE_MIDI::MIDI_EV
MidiEvent(uint64_t                timestamp,
          libremidi::message_type type = libremidi::message_type::AFTERTOUCH)
{
    PDJE_MIDI::MIDI_EV event{};
    event.type            = static_cast<uint8_t>(type);
    event.value           = 127;
    event.highres_time    = timestamp;
    constexpr char port[] = "dummy-midi";
    std::memcpy(event.port_name, port, sizeof(port));
    event.port_name_len = sizeof(port) - 1;
    return event;
}

void
ConfigureNoteRails(Judge_Init &init)
{
    init.SetRail("dummy-midi",
                 42,
                 static_cast<uint8_t>(libremidi::message_type::NOTE_ON),
                 0);
    init.SetRail("dummy-midi",
                 42,
                 static_cast<uint8_t>(libremidi::message_type::NOTE_OFF),
                 0);
    init.lambdas.use_event_sleep_time  = std::chrono::milliseconds(1);
    init.lambdas.miss_event_sleep_time = std::chrono::milliseconds(1);
}

template <int Buffer>
P_NOTE_VEC
AllNotes(OBJ &notes, uint64_t rail = 42)
{
    P_NOTE_VEC found;
    notes.Get<Buffer>((std::numeric_limits<LOCAL_TIME>::max)(), rail, found);
    return found;
}

} // namespace

TEST_CASE("midi pipeline: native attachment accepts either buffer and ignores "
          "empty lines")
{
    // Opaque identity only: never construct, dereference, or poll an IPC arena.
    alignas(PDJE_IPC::PDJE_Input_Transfer) std::byte arena_token{};
    auto                                            *arena =
        reinterpret_cast<PDJE_IPC::PDJE_Input_Transfer *>(&arena_token);
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> midi(4);

    for (const bool input_on : { false, true }) {
        for (const bool midi_on : { false, true }) {
            CAPTURE(input_on);
            CAPTURE(midi_on);
            Judge_Init           init;
            PDJE_INPUT_DATA_LINE line{};
            line.input_arena = input_on ? arena : nullptr;
            line.midi_datas  = midi_on ? &midi : nullptr;
            init.SetInputLine(line);
            REQUIRE(init.inputline.has_value() == (input_on || midi_on));
            if (init.inputline) {
                CHECK(init.inputline->input_arena == line.input_arena);
                CHECK(init.inputline->midi_datas == line.midi_datas);
                init.SetInputLine({});
                REQUIRE(init.inputline.has_value());
                CHECK(init.inputline->input_arena == line.input_arena);
                CHECK(init.inputline->midi_datas == line.midi_datas);

                // Replacing a mixed/input line with MIDI-only clears the arena.
                init.SetInputLine({ nullptr, &midi });
                CHECK(init.inputline->input_arena == nullptr);
                CHECK(init.inputline->midi_datas == &midi);
            }
        }
    }
}

TEST_CASE("midi pipeline: attached MIDI-only line traverses preprocessing and "
          "judge loop")
{
    CoreFixture                                core;
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> buffer(4);
    Judge_Init                                 init;
    core.Configure(init, buffer);
    REQUIRE(init.inputline.has_value());
    REQUIRE(init.inputline->input_arena == nullptr);
    std::vector<AxisSample> samples;
    init.lambdas.midi_axis = [&](LOCAL_TIME time, uint64_t rail, double value) {
        samples.push_back({ time, rail, value });
    };
    Judge_Loop loop(init);
    loop.loop_once(); // Empty buffer must not touch the missing input arena.
    CHECK(samples.empty());

    PDJE_MIDI::MIDI_EV event{};
    event.type  = static_cast<uint8_t>(libremidi::message_type::AFTERTOUCH);
    event.value = 127;
    event.highres_time    = 1001234;
    constexpr char port[] = "dummy-midi";
    std::memcpy(event.port_name, port, sizeof(port));
    event.port_name_len = sizeof(port) - 1;
    buffer.Write(event);
    loop.loop_once();
    REQUIRE(samples.size() == 1);
    CHECK(samples[0].time == 1234);
    CHECK(samples[0].rail == 42);
    CHECK(samples[0].value == 1.0);
    loop.loop_once();
    CHECK(samples.size() == 1); // No stale replay on an empty poll.
}

TEST_CASE(
    "midi pipeline: native judge starts ends and restarts with dummy MIDI only")
{
    auto backend = std::make_shared<DummyMidiBackend>();
    // Producer and callback state outlive the judge, including assertion
    // failures.
    std::unique_ptr<PDJE_MIDI::MIDI> midi;
    CoreFixture                      core;
    core.sync.store(audioSyncData{ .microsecond = 1 });
    std::promise<AxisSample> delivered;
    std::atomic<int>         callbacks{ 0 };
    JUDGE                    judge;

    core.Attach(judge.inits);
    judge.inits.SetInputLine({});
    CHECK(judge.Start() == JUDGE_STATUS::INPUT_LINE_IS_MISSING);
    judge.End();

    for (int run = 0; run < 2; ++run) {
        CAPTURE(run);
        delivered   = std::promise<AxisSample>{};
        auto result = delivered.get_future();
        callbacks.store(0);
        midi = std::make_unique<PDJE_MIDI::MIDI>(4, backend);
        midi->Config(backend->ports.front());
        midi->Run();
        core.Configure(judge.inits, midi->GetEventBuffer());
        REQUIRE(judge.inits.inputline.has_value());
        REQUIRE(judge.inits.inputline->input_arena == nullptr);
        judge.inits.lambdas.midi_axis =
            [&](LOCAL_TIME time, uint64_t rail, double value) {
                if (callbacks.fetch_add(1) == 0) {
                    delivered.set_value({ time, rail, value });
                }
            };
        judge.inits.lambdas.use_event_sleep_time = std::chrono::milliseconds(1);
        judge.inits.lambdas.miss_event_sleep_time =
            std::chrono::milliseconds(1);
        REQUIRE(judge.Start() == JUDGE_STATUS::OK);
        CHECK(judge.Start() == JUDGE_STATUS::ALREADY_RUNNING);
        backend->Emit("dummy-midi", { 0xD0, 127 });
        const auto status = result.wait_for(std::chrono::seconds(3));
        judge.End(); // Join all consumers before releasing the producer.
        REQUIRE(status == std::future_status::ready);
        const auto sample = result.get();
        CHECK(sample.time >= 0);
        CHECK(sample.rail == 42);
        CHECK(sample.value == 1.0);
        CHECK(callbacks.load() == 1);
        CHECK_FALSE(judge.inits.inputline.has_value());
        CHECK(backend->active_connections == 1);
        midi.reset();
        CHECK(backend->active_connections == 0);
        CHECK(backend->closed_connections == run + 1);
        backend->Emit("dummy-midi", { 0xD0, 0 });
        CHECK(callbacks.load() == 1);
        judge.End(); // Repeated shutdown is harmless.
    }
}

TEST_CASE(
    "judge suspension: MIDI polls drain without note, miss or axis production")
{
    CoreFixture                                core;
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> buffer(8);
    Judge_Init                                 init;
    core.Configure(init, buffer);
    ConfigureNoteRails(init);
    NOTE note;
    note.microsecond = 1234;
    init.note_objects->Fill<BUFFER_MAIN>(note, 42);
    init.note_objects->Fill<BUFFER_SUB>(note, 42);
    init.note_objects->Sort();
    const auto main = AllNotes<BUFFER_MAIN>(*init.note_objects);
    const auto sub  = AllNotes<BUFFER_SUB>(*init.note_objects);
    REQUIRE(main.size() == 1);
    REQUIRE(sub.size() == 1);
    std::atomic<int> used{ 0 }, missed{ 0 };
    int              axes     = 0;
    init.lambdas.used_event   = [&](uint64_t, bool, bool, uint64_t) { ++used; };
    init.lambdas.missed_event = [&](auto) { ++missed; };
    init.lambdas.midi_axis    = [&](LOCAL_TIME, uint64_t, double) { ++axes; };
    Judge_Loop loop(init);
    loop.StartEventLoop();
    REQUIRE(loop.SuspendJudgments());
    CHECK(loop.IsSuspended());
    buffer.Write(MidiEvent(1001234, libremidi::message_type::NOTE_ON));
    buffer.Write(MidiEvent(1001234, libremidi::message_type::NOTE_OFF));
    buffer.Write(MidiEvent(1001234));
    loop.loop_once();
    CHECK(buffer.Get()->empty()); // Manual polling is the sole MIDI consumer.
    CHECK_FALSE(main[0]->used);
    CHECK_FALSE(sub[0]->used);
    // An empty poll at a much later playback time must not cut misses either.
    core.sync.store(
        audioSyncData{ .consumed_frames = 48000, .microsecond = 2000000 });
    loop.loop_once();
    CHECK_FALSE(main[0]->used);
    CHECK_FALSE(sub[0]->used);
    core.sync.store(audioSyncData{}); // Facade invalidates stopped Core sync.
    REQUIRE(loop.ResumeJudgments());
    loop.loop_once();
    loop.EndEventLoop();
    CHECK(used.load() == 0);
    CHECK(missed.load() == 0);
    CHECK(axes == 0);
}

TEST_CASE(
    "judge raw cutoff precedes device calibration for both parser overloads")
{
    RAIL_DB rails;
    rails.offset["dummy-midi"] = 1000000;
    PARSE_OUT parsed{};
    MIDI_RAW  midi{ MidiEvent(99), MidiEvent(100), MidiEvent(101) };
    Parse(parsed, rails, midi, 100);
    REQUIRE(parsed.midi_logs.size() == 1);
    CHECK(parsed.midi_logs[0].highres_time == 1000101);
    Parse(parsed, rails, MIDI_RAW{ MidiEvent(99) }, 100);
    CHECK(parsed.midi_logs.empty()); // All filtered: no front()/back() access.
    Parse(parsed, rails, midi);
    CHECK(parsed.midi_logs.size() == 3); // Default cutoff remains compatible.

    PDJE_Input_Log log{};
    constexpr char name[] = "dummy-midi";
    std::memcpy(log.name, name, sizeof(name));
    log.name_len = sizeof(name) - 1;
    INPUT_RAW raw;
    for (uint64_t time : { 99, 100, 101 }) {
        log.microSecond = time;
        raw.push_back(log);
    }
    Parse(parsed, rails, raw, 100);
    REQUIRE(parsed.logs.size() == 1);
    CHECK(parsed.logs[0].microSecond == 1000101);
    Parse(parsed, rails, INPUT_RAW{ raw.front() }, 100);
    CHECK(parsed.logs.empty());
    Parse(parsed, rails, raw);
    CHECK(parsed.logs.size() == 3);
}

TEST_CASE(
    "judge resume: delayed old MIDI is rejected before a large positive offset")
{
    CoreFixture core;
    core.sync.store(audioSyncData{ .microsecond = 1 });
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> buffer(8);
    Judge_Init                                 init;
    core.Configure(init, buffer);
    ConfigureNoteRails(init);
    constexpr int64_t offset         = 1000000000;
    init.raildb.offset["dummy-midi"] = offset;
    std::vector<AxisSample> samples;
    init.lambdas.midi_axis = [&](LOCAL_TIME time, uint64_t rail, double value) {
        samples.push_back({ time, rail, value });
    };
    Judge_Loop                loop(init);
    PDJE_HIGHRES_CLOCK::CLOCK clock;
    REQUIRE(loop.SuspendJudgments());
    const auto paused_time = clock.Get_MicroSecond();
    REQUIRE(loop.ResumeJudgments());
    CHECK_FALSE(loop.IsSuspended());
    // Simulate delayed transport: the paused event arrives AFTER resume.
    buffer.Write(MidiEvent(paused_time));
    loop.loop_once();
    CHECK(samples.empty());
    // Synthetic next tick makes the test independent of clock granularity.
    const auto fresh_time = clock.Get_MicroSecond() + 1;
    buffer.Write(MidiEvent(fresh_time));
    loop.loop_once();
    REQUIRE(samples.size() == 1);
    CHECK(samples[0].time == static_cast<LOCAL_TIME>(fresh_time + offset - 1));
    loop.loop_once();
    CHECK(samples.size() == 1);
}

TEST_CASE("judge preprocessing: collection alone cannot mutate notes")
{
    CoreFixture                                core;
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> buffer(4);
    Judge_Init                                 init;
    core.Configure(init, buffer);
    NOTE note;
    note.microsecond = 1234;
    init.note_objects->Fill<BUFFER_MAIN>(note, 42);
    init.note_objects->Sort();
    const auto notes = AllNotes<BUFFER_MAIN>(*init.note_objects);
    REQUIRE(notes.size() == 1);
    PreProcess pre(&init);
    buffer.Write(MidiEvent(2000000));
    const bool collected = pre.CollectInputs();
    REQUIRE(collected);
    CHECK_FALSE(notes[0]->used);
    CHECK(pre.ProcessCollected(collected));
    CHECK(notes[0]->used);   // Miss production only occurs during processing.
    CHECK_FALSE(pre.Work()); // Existing single-call API still clears the batch.
    CHECK(pre.parsed_res.midi_logs.empty());
}

TEST_CASE(
    "judge restart: resets both buffers and cursors without replacing setup")
{
    CoreFixture core;
    core.sync.store(audioSyncData{}); // No wall-clock dependency or production.
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> buffer(4);
    JUDGE                                      judge;
    CHECK_FALSE(judge.ResetForRestart());
    CHECK_FALSE(judge.SuspendJudgments());
    CHECK_FALSE(judge.ResumeJudgments());
    CHECK_FALSE(judge.IsSuspended());
    core.Configure(judge.inits, buffer);
    ConfigureNoteRails(judge.inits);
    judge.inits.lambdas.midi_axis = [](LOCAL_TIME, uint64_t, double) {};
    for (uint64_t rail : { 42, 43 }) {
        for (LOCAL_TIME time : { 1234, 5678 }) {
            NOTE note;
            note.type        = "preserved";
            note.microsecond = time;
            judge.inits.note_objects->Fill<BUFFER_MAIN>(note, rail);
            judge.inits.note_objects->Fill<BUFFER_SUB>(note, rail);
        }
    }
    judge.inits.note_objects->Sort();
    bool runtime = false;
    SUBCASE("without a runtime")
    {
    }
    SUBCASE("with polling runtime")
    {
        runtime = true;
    }
    if (runtime) {
        REQUIRE(judge.Start() == JUDGE_STATUS::OK);
        REQUIRE(judge.SuspendJudgments());
    }
    auto               *owner = &*judge.inits.note_objects;
    std::vector<NOTE *> original;
    for (uint64_t rail : { 42, 43 }) {
        const auto main = AllNotes<BUFFER_MAIN>(*owner, rail);
        const auto sub  = AllNotes<BUFFER_SUB>(*owner, rail);
        REQUIRE(main.size() == 2);
        REQUIRE(sub.size() == 2);
        for (NOTE *note : main) {
            note->used = true;
            original.push_back(note);
        }
        for (NOTE *note : sub) {
            note->used = true;
            original.push_back(note);
        }
        // Force both NOTE_ITR cursors to end, rather than testing only flags.
        CHECK(AllNotes<BUFFER_MAIN>(*owner, rail).empty());
        CHECK(AllNotes<BUFFER_SUB>(*owner, rail).empty());
    }
    REQUIRE(judge.ResetForRestart());
    CHECK(judge.IsRunning() == runtime);
    CHECK(judge.IsSuspended() == runtime);
    CHECK(&*judge.inits.note_objects == owner);
    CHECK(judge.inits.inputline->midi_datas == &buffer);
    CHECK(judge.inits.coreline->syncD == &core.sync);
    CHECK(judge.inits.ev_rule->use_range_microsecond == 100000);
    CHECK_FALSE(judge.inits.raildb.Empty());
    CHECK(static_cast<bool>(judge.inits.lambdas.midi_axis));
    for (NOTE *note : original) {
        CHECK_FALSE(note->used);
        CHECK(note->type == "preserved");
    }
    for (uint64_t rail : { 42, 43 }) {
        const auto main = AllNotes<BUFFER_MAIN>(*owner, rail);
        const auto sub  = AllNotes<BUFFER_SUB>(*owner, rail);
        REQUIRE(main.size() == 2);
        REQUIRE(sub.size() == 2);
        CHECK(main[0]->microsecond == 1234);
        CHECK(sub[1]->microsecond == 5678);
    }
    REQUIRE(judge.ResetForRestart()); // Repeated restart stays gated.
    if (runtime) {
        REQUIRE(judge.ResumeJudgments());
        CHECK(judge.IsRunning());
        CHECK_FALSE(judge.IsSuspended());
        REQUIRE(judge.SuspendJudgments());
    }
    judge.End();
}

TEST_CASE("judge suspension waits for an in-flight direct axis callback")
{
    CoreFixture                                core;
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> buffer(4);
    Judge_Init                                 init;
    core.Configure(init, buffer);
    std::promise<void> entered, release, suspending;
    auto               entered_result    = entered.get_future();
    auto               released          = release.get_future().share();
    auto               suspending_result = suspending.get_future();
    std::atomic<bool>  callback_timed_out{ false };
    init.lambdas.midi_axis = [&](LOCAL_TIME, uint64_t, double) {
        entered.set_value();
        callback_timed_out = released.wait_for(std::chrono::seconds(3)) !=
                             std::future_status::ready;
    };
    Judge_Loop loop(init);
    buffer.Write(MidiEvent(1001234));
    auto       poll = std::async(std::launch::async, [&] { loop.loop_once(); });
    const auto entered_status =
        entered_result.wait_for(std::chrono::seconds(3));
    if (entered_status != std::future_status::ready) {
        release.set_value();
        REQUIRE(entered_status == std::future_status::ready);
    }
    auto suspend = std::async(std::launch::async, [&] {
        suspending.set_value();
        return loop.SuspendJudgments();
    });
    CHECK(suspending_result.wait_for(std::chrono::seconds(3)) ==
          std::future_status::ready);
    CHECK(suspend.wait_for(std::chrono::milliseconds(20)) ==
          std::future_status::timeout);
    release.set_value(); // Always release before any fatal assertion or join.
    REQUIRE(poll.wait_for(std::chrono::seconds(3)) ==
            std::future_status::ready);
    poll.get();
    REQUIRE(suspend.wait_for(std::chrono::seconds(3)) ==
            std::future_status::ready);
    CHECK(suspend.get());
    CHECK(loop.IsSuspended());
    CHECK_FALSE(callback_timed_out.load());
}

TEST_CASE("judge restart joins old callbacks and discards queued work before "
          "rearming")
{
    bool test_miss = false;
    SUBCASE("use worker")
    {
    }
    SUBCASE("miss worker")
    {
        test_miss = true;
    }
    CoreFixture                                core;
    Spinlock_Double_Buffer<PDJE_MIDI::MIDI_EV> buffer(8);
    Judge_Init                                 init;
    core.Configure(init, buffer);
    ConfigureNoteRails(init);
    for (LOCAL_TIME time : { 10000, 250000 }) {
        NOTE note;
        note.microsecond = time;
        init.note_objects->Fill<BUFFER_MAIN>(note, 42);
        init.note_objects->Fill<BUFFER_SUB>(note, 42);
    }
    init.note_objects->Sort();
    const auto main = AllNotes<BUFFER_MAIN>(*init.note_objects);
    const auto sub  = AllNotes<BUFFER_SUB>(*init.note_objects);
    REQUIRE(main.size() == 2);
    REQUIRE(sub.size() == 2);
    std::promise<void> entered, release, resetting, fresh_delivered;
    auto               entered_result   = entered.get_future();
    auto               released         = release.get_future().share();
    auto               resetting_result = resetting.get_future();
    auto               fresh_result     = fresh_delivered.get_future();
    std::atomic<int>   callbacks{ 0 }, fresh_callbacks{ 0 };
    std::atomic<bool>  callback_timed_out{ false },
        old_notes_still_used{ false };
    std::atomic<bool> after_reset{ false }, stale_delivery{ false };
    auto              callback = [&](bool fresh) {
        if (callbacks.fetch_add(1) == 0) {
            entered.set_value();
            callback_timed_out   = released.wait_for(std::chrono::seconds(3)) !=
                                   std::future_status::ready;
            old_notes_still_used = main[0]->used && sub[0]->used;
        }
        if (after_reset.load()) {
            if (!fresh) {
                stale_delivery = true;
            } else if (fresh_callbacks.fetch_add(1) == 1) {
                fresh_delivered.set_value();
            }
        }
    };
    init.lambdas.used_event = [&](uint64_t, bool, bool, uint64_t diff) {
        callback(diff == 1);
    };
    init.lambdas.missed_event = [&](auto notes) {
        callback(notes.at(42).front().microsecond == 10000);
    };
    Judge_Loop loop(init);
    loop.StartEventLoop();
    auto produce = [&](uint64_t raw_time) {
        if (test_miss) {
            buffer.Write(MidiEvent(raw_time));
        } else {
            buffer.Write(MidiEvent(raw_time, libremidi::message_type::NOTE_ON));
            buffer.Write(
                MidiEvent(raw_time, libremidi::message_type::NOTE_OFF));
        }
        loop.loop_once();
    };
    produce(test_miss ? 1200000 : 1010000);
    const auto entered_status =
        entered_result.wait_for(std::chrono::seconds(3));
    if (entered_status != std::future_status::ready) {
        release.set_value();
        REQUIRE(entered_status == std::future_status::ready);
    }
    // The worker owns one read side while new jobs enter the write side.
    produce(test_miss ? 1500000 : 1250000);
    auto reset = std::async(std::launch::async, [&] {
        resetting.set_value();
        return loop.ResetForRestart();
    });
    CHECK(resetting_result.wait_for(std::chrono::seconds(3)) ==
          std::future_status::ready);
    CHECK(reset.wait_for(std::chrono::milliseconds(20)) ==
          std::future_status::timeout);
    release.set_value();
    REQUIRE(reset.wait_for(std::chrono::seconds(3)) ==
            std::future_status::ready);
    REQUIRE(reset.get());
    CHECK_FALSE(callback_timed_out.load());
    CHECK(old_notes_still_used.load());
    CHECK(loop.IsSuspended());
    for (NOTE *note : main) {
        CHECK_FALSE(note->used);
    }
    for (NOTE *note : sub) {
        CHECK_FALSE(note->used);
    }
    CHECK(AllNotes<BUFFER_MAIN>(*init.note_objects).size() == 2);
    CHECK(AllNotes<BUFFER_SUB>(*init.note_objects).size() == 2);
    after_reset = true;
    core.sync.store(audioSyncData{});
    REQUIRE(loop.ResumeJudgments()); // Recreates both stopped event workers.
    const uint64_t            local_time = test_miss ? 210001 : 10001;
    PDJE_HIGHRES_CLOCK::CLOCK clock;
    const auto raw_time = clock.Get_MicroSecond() + local_time + 1;
    core.sync.store(audioSyncData{ .microsecond = raw_time - local_time });
    produce(raw_time);
    const auto fresh_status = fresh_result.wait_for(std::chrono::seconds(3));
    loop.EndEventLoop();
    CHECK(fresh_status == std::future_status::ready);
    CHECK(fresh_callbacks.load() == 2);
    CHECK_FALSE(stale_delivery.load());
}
