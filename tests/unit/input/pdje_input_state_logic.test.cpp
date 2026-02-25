#include "PDJE_Input_StateLogic.hpp"
#include <doctest/doctest.h>

TEST_CASE("input/state: sanitize removes invalid devices and preserves order")
{
    std::vector<DeviceData> in = {
        { PDJE_Dev_Type::KEYBOARD, "kbd0", "dev://kbd0" },
        { PDJE_Dev_Type::UNKNOWN, "bad-type", "dev://bad" },
        { PDJE_Dev_Type::MOUSE, "", "dev://mouse0" },
        { PDJE_Dev_Type::MOUSE, "mouse1", "" },
        { PDJE_Dev_Type::MOUSE, "mouse2", "dev://mouse2" }
    };

    const auto out = PDJE_INPUT_STATE_LOGIC::SanitizeConfigDevices(in);
    REQUIRE(out.size() == 2);
    CHECK(out[0].Name == "kbd0");
    CHECK(out[0].device_specific_id == "dev://kbd0");
    CHECK(out[1].Name == "mouse2");
    CHECK(out[1].device_specific_id == "dev://mouse2");
}

TEST_CASE("input/state: init/config/run preconditions are state-specific")
{
    CHECK(PDJE_INPUT_STATE_LOGIC::CanInit(PDJE_INPUT_STATE::DEAD));
    CHECK_FALSE(
        PDJE_INPUT_STATE_LOGIC::CanInit(PDJE_INPUT_STATE::DEVICE_CONFIG_STATE));

    CHECK(PDJE_INPUT_STATE_LOGIC::CanConfig(
        PDJE_INPUT_STATE::DEVICE_CONFIG_STATE));
    CHECK_FALSE(
        PDJE_INPUT_STATE_LOGIC::CanConfig(PDJE_INPUT_STATE::INPUT_LOOP_READY));

    CHECK(PDJE_INPUT_STATE_LOGIC::CanRun(PDJE_INPUT_STATE::INPUT_LOOP_READY));
    CHECK_FALSE(
        PDJE_INPUT_STATE_LOGIC::CanRun(PDJE_INPUT_STATE::INPUT_LOOP_RUNNING));
}

TEST_CASE("input/state: config decision matrix covers input, midi fallback, failures")
{
    SUBCASE("valid input and backend config success")
    {
        const auto r = PDJE_INPUT_STATE_LOGIC::DecideConfigOutcome(
            true, false, true);
        CHECK(r.success);
        CHECK(r.flag_input_on);
        CHECK(r.next_state == PDJE_INPUT_STATE::INPUT_LOOP_READY);
        CHECK_FALSE(r.should_call_kill);
        CHECK_FALSE(r.backend_fail_path);
    }

    SUBCASE("valid input but backend config failure")
    {
        const auto r = PDJE_INPUT_STATE_LOGIC::DecideConfigOutcome(
            true, true, false);
        CHECK_FALSE(r.success);
        CHECK_FALSE(r.flag_input_on);
        CHECK(r.next_state == PDJE_INPUT_STATE::DEVICE_CONFIG_STATE);
        CHECK_FALSE(r.should_call_kill);
        CHECK(r.backend_fail_path);
    }

    SUBCASE("no valid input but midi exists uses midi-only fallback path")
    {
        const auto r = PDJE_INPUT_STATE_LOGIC::DecideConfigOutcome(
            false, true, false);
        CHECK(r.success);
        CHECK_FALSE(r.flag_input_on);
        CHECK(r.next_state == PDJE_INPUT_STATE::INPUT_LOOP_READY);
        CHECK(r.should_call_kill);
        CHECK_FALSE(r.backend_fail_path);
    }

    SUBCASE("no valid input and no midi is failure")
    {
        const auto r = PDJE_INPUT_STATE_LOGIC::DecideConfigOutcome(
            false, false, false);
        CHECK_FALSE(r.success);
        CHECK_FALSE(r.flag_input_on);
        CHECK(r.next_state == PDJE_INPUT_STATE::DEVICE_CONFIG_STATE);
        CHECK_FALSE(r.should_call_kill);
        CHECK_FALSE(r.backend_fail_path);
    }
}

TEST_CASE("input/state: kill decision classification matches PDJE_Input kill branches")
{
    using PDJE_INPUT_STATE_LOGIC::ClassifyKillAction;
    using PDJE_INPUT_STATE_LOGIC::KillAction;

    CHECK(ClassifyKillAction(PDJE_INPUT_STATE::DEAD) == KillAction::NoOp);
    CHECK(ClassifyKillAction(PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) ==
          KillAction::BackendKill);
    CHECK(ClassifyKillAction(PDJE_INPUT_STATE::INPUT_LOOP_READY) ==
          KillAction::TerminateLoop);
    CHECK(ClassifyKillAction(PDJE_INPUT_STATE::INPUT_LOOP_RUNNING) ==
          KillAction::TerminateLoop);
}
