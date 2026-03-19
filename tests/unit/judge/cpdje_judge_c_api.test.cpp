#include <doctest/doctest.h>

#include "CPDJE_Input.h"
#include "CPDJE_Judge.h"
#include "CPDJE_interface.h"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <string>
#include <thread>

namespace {

namespace fs = std::filesystem;

fs::path
make_temp_root(const std::string &label)
{
    return fs::temp_directory_path() /
           ("pdje_judge_c_abi_" + label + "_" +
            std::to_string(
                std::chrono::steady_clock::now().time_since_epoch().count()));
}

struct ScopedCleanup {
    fs::path root;

    ~ScopedCleanup()
    {
        std::error_code ec;
        fs::remove_all(root, ec);
    }
};

void PDJE_CALL
count_used_events(const PDJE_JudgeUsedEventV1 *event, void *user_data)
{
    if (event == nullptr || user_data == nullptr) {
        return;
    }
    auto *count = static_cast<std::atomic<int> *>(user_data);
    count->fetch_add(1, std::memory_order_relaxed);
}

void PDJE_CALL
count_missed_notes(const PDJE_JudgeMissedNoteV1 *notes,
                   size_t                        note_count,
                   void                         *user_data)
{
    if (notes == nullptr || note_count == 0 || user_data == nullptr) {
        return;
    }
    auto *count = static_cast<std::atomic<int> *>(user_data);
    count->fetch_add(static_cast<int>(note_count), std::memory_order_relaxed);
}

bool
wait_for_nonzero(const std::atomic<int> &value, const std::chrono::milliseconds timeout)
{
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (value.load(std::memory_order_relaxed) > 0) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return value.load(std::memory_order_relaxed) > 0;
}

bool
find_first_real_device(const PDJE_InputDeviceListHandleV1 *devices,
                       size_t                              &out_index,
                       PDJE_InputDeviceTypeV1             &out_type)
{
    const auto count = pdje_input_device_list_size_v1(devices);
    for (size_t i = 0; i < count; ++i) {
        PDJE_InputDeviceViewV1 view {};
        view.struct_size = sizeof(view);
        if (pdje_input_device_list_get_v1(devices, i, &view) !=
            PDJE_INPUT_RESULT_OK_V1) {
            continue;
        }
        if (view.type == PDJE_INPUT_DEVICE_MOUSE_V1 ||
            view.type == PDJE_INPUT_DEVICE_KEYBOARD_V1) {
            out_index = i;
            out_type  = view.type;
            return true;
        }
    }
    return false;
}

} // namespace

TEST_CASE("CPDJE judge C ABI validates null arguments and safe no-op teardown")
{
    CHECK(pdje_judge_create_v1(nullptr) == PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_judge_attach_engine_v1(nullptr, nullptr) ==
          PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_judge_attach_input_v1(nullptr, nullptr) ==
          PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_judge_set_event_rule_v1(nullptr, 0, 0) ==
          PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_judge_start_v1(nullptr, nullptr) ==
          PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1);

    pdje_judge_end_v1(nullptr);
    pdje_judge_destroy_v1(nullptr);
}

TEST_CASE("CPDJE judge C ABI stages prerequisites and rejects out-of-range rails")
{
    PDJE_JudgeHandleV1 *judge = nullptr;
    REQUIRE(pdje_judge_create_v1(&judge) == PDJE_JUDGE_RESULT_OK_V1);
    REQUIRE(judge != nullptr);

    PDJE_JudgeStartStatusV1 status = PDJE_JUDGE_START_STATUS_OK_V1;
    REQUIRE(pdje_judge_start_v1(judge, &status) == PDJE_JUDGE_RESULT_OK_V1);
    CHECK(status == PDJE_JUDGE_START_STATUS_CORE_LINE_MISSING_V1);

    const auto root = make_temp_root("staged");
    ScopedCleanup cleanup { root };

    PDJE_EngineHandleV1 *engine = nullptr;
    REQUIRE(pdje_engine_create_v1(root.generic_string().c_str(), &engine) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(engine != nullptr);

    const int manual_init = pdje_engine_init_player_manual_v1(engine, 48);
    CHECK((manual_init == PDJE_RESULT_OK_V1 ||
           manual_init == PDJE_RESULT_INTERNAL_ERROR_V1));
    REQUIRE(pdje_judge_attach_engine_v1(judge, engine) == PDJE_JUDGE_RESULT_OK_V1);

    if (manual_init == PDJE_RESULT_OK_V1) {
        REQUIRE(pdje_judge_start_v1(judge, &status) == PDJE_JUDGE_RESULT_OK_V1);
        CHECK(status == PDJE_JUDGE_START_STATUS_INPUT_LINE_MISSING_V1);
    }

    PDJE_InputHandleV1 *input = nullptr;
    REQUIRE(pdje_input_create_v1(&input) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(input != nullptr);
    REQUIRE(pdje_input_init_v1(input, nullptr, nullptr, 0) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(pdje_judge_attach_input_v1(judge, input) == PDJE_JUDGE_RESULT_OK_V1);

    PDJE_InputDeviceListHandleV1 *devices = nullptr;
    REQUIRE(pdje_input_list_devices_v1(input, &devices) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(devices != nullptr);

    size_t                 device_index    = 0;
    PDJE_InputDeviceTypeV1 device_type     = PDJE_INPUT_DEVICE_UNKNOWN_V1;
    const bool             has_real_device =
        find_first_real_device(devices, device_index, device_type);
    if (manual_init == PDJE_RESULT_OK_V1 && has_real_device) {
        const size_t selected_index = device_index;
        const auto config_result =
            pdje_input_config_v1(input, devices, &selected_index, 1, nullptr, nullptr, 0);
        if (config_result == PDJE_INPUT_RESULT_OK_V1) {
            REQUIRE(pdje_judge_start_v1(judge, &status) == PDJE_JUDGE_RESULT_OK_V1);
            CHECK(status == PDJE_JUDGE_START_STATUS_NOTE_OBJECT_MISSING_V1);

            const uint16_t device_key_mask =
                device_type == PDJE_INPUT_DEVICE_MOUSE_V1 ? PDJE_MOUSE_L_BTN_DOWN : 1;
            REQUIRE(pdje_judge_add_input_rail_v1(
                        judge, devices, device_index, device_key_mask, 0, 7) ==
                    PDJE_JUDGE_RESULT_OK_V1);
            CHECK(pdje_judge_add_input_rail_v1(
                      judge, devices, device_index + 1000, 1, 0, 9) ==
                  PDJE_JUDGE_RESULT_OUT_OF_RANGE_V1);

            REQUIRE(pdje_judge_start_v1(judge, &status) == PDJE_JUDGE_RESULT_OK_V1);
            CHECK(status == PDJE_JUDGE_START_STATUS_NOTE_OBJECT_MISSING_V1);

            CHECK(pdje_judge_add_note_object_v1(
                      judge, "tap", 0, "", "", "", 0, 0, 9999) ==
                  PDJE_JUDGE_RESULT_OUT_OF_RANGE_V1);
            REQUIRE(pdje_judge_add_note_object_v1(
                        judge, "tap", 0, "", "", "", 0, 0, 7) ==
                    PDJE_JUDGE_RESULT_OK_V1);

            REQUIRE(pdje_judge_start_v1(judge, &status) == PDJE_JUDGE_RESULT_OK_V1);
            CHECK(status == PDJE_JUDGE_START_STATUS_EVENT_RULE_EMPTY_V1);
        } else {
            INFO("input device configuration is unavailable in this environment");
        }
    }

    pdje_input_device_list_destroy_v1(devices);
    CHECK(pdje_input_kill_v1(input) == PDJE_INPUT_RESULT_OK_V1);
    pdje_input_destroy_v1(input);
    pdje_engine_reset_player_v1(engine);
    pdje_engine_destroy_v1(engine);
    pdje_judge_destroy_v1(judge);
}

TEST_CASE("CPDJE judge C ABI can start with configured engine/input and emit missed callbacks")
{
    const auto root = make_temp_root("runtime");
    ScopedCleanup cleanup { root };

    PDJE_JudgeHandleV1 *judge = nullptr;
    REQUIRE(pdje_judge_create_v1(&judge) == PDJE_JUDGE_RESULT_OK_V1);
    REQUIRE(judge != nullptr);

    PDJE_EngineHandleV1 *engine = nullptr;
    REQUIRE(pdje_engine_create_v1(root.generic_string().c_str(), &engine) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(engine != nullptr);

    if (pdje_engine_init_player_manual_v1(engine, 48) != PDJE_RESULT_OK_V1) {
        INFO("manual player init is unavailable in this environment");
        pdje_engine_destroy_v1(engine);
        pdje_judge_destroy_v1(judge);
        return;
    }
    REQUIRE(pdje_judge_attach_engine_v1(judge, engine) == PDJE_JUDGE_RESULT_OK_V1);

    PDJE_InputHandleV1 *input = nullptr;
    REQUIRE(pdje_input_create_v1(&input) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(input != nullptr);
    REQUIRE(pdje_input_init_v1(input, nullptr, nullptr, 0) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(pdje_judge_attach_input_v1(judge, input) == PDJE_JUDGE_RESULT_OK_V1);

    PDJE_InputDeviceListHandleV1 *devices = nullptr;
    REQUIRE(pdje_input_list_devices_v1(input, &devices) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(devices != nullptr);

    size_t                 device_index = 0;
    PDJE_InputDeviceTypeV1 device_type  = PDJE_INPUT_DEVICE_UNKNOWN_V1;
    if (!find_first_real_device(devices, device_index, device_type)) {
        INFO("no keyboard or mouse device is available for judge runtime setup");
        pdje_input_device_list_destroy_v1(devices);
        CHECK(pdje_input_kill_v1(input) == PDJE_INPUT_RESULT_OK_V1);
        pdje_input_destroy_v1(input);
        pdje_engine_reset_player_v1(engine);
        pdje_engine_destroy_v1(engine);
        pdje_judge_destroy_v1(judge);
        return;
    }

    const auto config_result =
        pdje_input_config_v1(input, devices, &device_index, 1, nullptr, nullptr, 0);
    if (config_result != PDJE_INPUT_RESULT_OK_V1) {
        INFO("input device configuration is unavailable in this environment");
        pdje_input_device_list_destroy_v1(devices);
        CHECK(pdje_input_kill_v1(input) == PDJE_INPUT_RESULT_OK_V1);
        pdje_input_destroy_v1(input);
        pdje_engine_reset_player_v1(engine);
        pdje_engine_destroy_v1(engine);
        pdje_judge_destroy_v1(judge);
        return;
    }

    REQUIRE(pdje_judge_set_event_rule_v1(judge, 0, 0) ==
            PDJE_JUDGE_RESULT_OK_V1);
    REQUIRE(pdje_judge_set_callback_intervals_v1(judge, 1, 1) ==
            PDJE_JUDGE_RESULT_OK_V1);

    const uint16_t device_key_mask =
        device_type == PDJE_INPUT_DEVICE_MOUSE_V1 ? PDJE_MOUSE_L_BTN_DOWN : 1;
    REQUIRE(pdje_judge_add_input_rail_v1(
                judge, devices, device_index, device_key_mask, 0, 7) ==
            PDJE_JUDGE_RESULT_OK_V1);
    REQUIRE(pdje_judge_add_note_object_v1(judge, "tap", 0, "", "", "", 0, 0, 7) ==
            PDJE_JUDGE_RESULT_OK_V1);

    std::atomic<int> used_count { 0 };
    std::atomic<int> missed_count { 0 };
    REQUIRE(pdje_judge_set_used_callback_v1(judge, count_used_events, &used_count) ==
            PDJE_JUDGE_RESULT_OK_V1);
    REQUIRE(pdje_judge_set_missed_callback_v1(judge, count_missed_notes, &missed_count) ==
            PDJE_JUDGE_RESULT_OK_V1);

    PDJE_JudgeStartStatusV1 status = PDJE_JUDGE_START_STATUS_NOTE_OBJECT_MISSING_V1;
    REQUIRE(pdje_judge_start_v1(judge, &status) == PDJE_JUDGE_RESULT_OK_V1);
    REQUIRE(status == PDJE_JUDGE_START_STATUS_OK_V1);

    CHECK(pdje_judge_set_event_rule_v1(judge, 10, 10) ==
          PDJE_JUDGE_RESULT_INVALID_STATE_V1);
    CHECK(pdje_judge_add_note_object_v1(judge, "tap", 0, "", "", "", 0, 0, 7) ==
          PDJE_JUDGE_RESULT_INVALID_STATE_V1);
    CHECK(pdje_judge_set_used_callback_v1(judge, nullptr, nullptr) ==
          PDJE_JUDGE_RESULT_INVALID_STATE_V1);
    CHECK(pdje_judge_set_callback_intervals_v1(judge, 10, 10) ==
          PDJE_JUDGE_RESULT_INVALID_STATE_V1);

    CHECK(wait_for_nonzero(missed_count, std::chrono::milliseconds(500)));
    CHECK(used_count.load(std::memory_order_relaxed) >= 0);

    pdje_judge_end_v1(judge);
    REQUIRE(pdje_judge_set_missed_callback_v1(judge, nullptr, nullptr) ==
            PDJE_JUDGE_RESULT_OK_V1);

    pdje_input_device_list_destroy_v1(devices);
    CHECK(pdje_input_kill_v1(input) == PDJE_INPUT_RESULT_OK_V1);
    pdje_input_destroy_v1(input);
    pdje_engine_reset_player_v1(engine);
    pdje_engine_destroy_v1(engine);
    pdje_judge_destroy_v1(judge);
}
