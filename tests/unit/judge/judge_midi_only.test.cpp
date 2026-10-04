#include "../input/dummy_midi_backend.hpp"
#include "PDJE_Judge.hpp"
#include <doctest/doctest.h>

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <future>
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
