#include <doctest/doctest.h>

#include "CPDJE_Input.h"

#include <string>

namespace {

std::string
to_string(const PDJE_InputStringViewV1 &value)
{
    if (value.data == nullptr || value.size == 0) {
        return {};
    }
    return std::string(value.data, value.size);
}

} // namespace

TEST_CASE("CPDJE input C ABI fresh handle reports dead state and safe empty snapshot")
{
    PDJE_InputHandleV1 *input = nullptr;
    REQUIRE(pdje_input_create_v1(&input) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(input != nullptr);

    PDJE_InputStateV1 state = PDJE_INPUT_STATE_LOOP_RUNNING_V1;
    REQUIRE(pdje_input_get_state_v1(input, &state) == PDJE_INPUT_RESULT_OK_V1);
    CHECK(state == PDJE_INPUT_STATE_DEAD_V1);

    PDJE_InputStringViewV1 backend {};
    REQUIRE(pdje_input_get_backend_name_v1(input, &backend) ==
            PDJE_INPUT_RESULT_OK_V1);
    CHECK(to_string(backend) == "none");

    PDJE_InputDeviceListHandleV1 *device_list = nullptr;
    CHECK(pdje_input_list_devices_v1(input, &device_list) ==
          PDJE_INPUT_RESULT_INVALID_STATE_V1);

    PDJE_MidiDeviceListHandleV1 *midi_list = nullptr;
    CHECK(pdje_input_list_midi_devices_v1(input, &midi_list) ==
          PDJE_INPUT_RESULT_INVALID_STATE_V1);

    CHECK(pdje_input_config_v1(input, nullptr, nullptr, 0, nullptr, nullptr, 0) ==
          PDJE_INPUT_RESULT_INVALID_STATE_V1);
    CHECK(pdje_input_run_v1(input) == PDJE_INPUT_RESULT_INVALID_STATE_V1);

    PDJE_InputSnapshotHandleV1 *snapshot = nullptr;
    REQUIRE(pdje_input_poll_snapshot_v1(input, &snapshot) ==
            PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(snapshot != nullptr);

    PDJE_InputSnapshotInfoV1 info {};
    info.struct_size = sizeof(info);
    REQUIRE(pdje_input_snapshot_describe_v1(snapshot, &info) ==
            PDJE_INPUT_RESULT_OK_V1);
    CHECK(info.has_input_stream == 0);
    CHECK(info.has_midi_stream == 0);
    CHECK(info.input_event_count == 0);
    CHECK(info.midi_event_count == 0);
    CHECK(pdje_input_snapshot_input_size_v1(snapshot) == 0);
    CHECK(pdje_input_snapshot_midi_size_v1(snapshot) == 0);
    CHECK(pdje_input_snapshot_input_get_v1(snapshot, 0, nullptr) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_input_snapshot_midi_get_v1(snapshot, 0, nullptr) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);

    pdje_input_snapshot_destroy_v1(snapshot);
    CHECK(pdje_input_kill_v1(input) == PDJE_INPUT_RESULT_OK_V1);
    pdje_input_destroy_v1(input);
    pdje_input_destroy_v1(nullptr);
    pdje_input_snapshot_destroy_v1(nullptr);
}

TEST_CASE("CPDJE input C ABI init, enumerate, validate getters, and kill cleanly")
{
    PDJE_InputHandleV1 *input = nullptr;
    REQUIRE(pdje_input_create_v1(&input) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(input != nullptr);

    REQUIRE(pdje_input_init_v1(input, nullptr, nullptr, 0) ==
            PDJE_INPUT_RESULT_OK_V1);

    PDJE_InputStateV1 state = PDJE_INPUT_STATE_DEAD_V1;
    REQUIRE(pdje_input_get_state_v1(input, &state) == PDJE_INPUT_RESULT_OK_V1);
    CHECK(state == PDJE_INPUT_STATE_DEVICE_CONFIG_V1);

    PDJE_InputStringViewV1 backend {};
    REQUIRE(pdje_input_get_backend_name_v1(input, &backend) ==
            PDJE_INPUT_RESULT_OK_V1);
    const bool backend_view_is_sane =
        backend.data != nullptr || backend.size == 0;
    CHECK(backend_view_is_sane);

    PDJE_InputDeviceListHandleV1 *device_list = nullptr;
    REQUIRE(pdje_input_list_devices_v1(input, &device_list) ==
            PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(device_list != nullptr);

    const auto device_count = pdje_input_device_list_size_v1(device_list);
    PDJE_InputDeviceViewV1 device_view {};
    device_view.struct_size = sizeof(device_view);
    if (device_count > 0) {
        REQUIRE(pdje_input_device_list_get_v1(device_list, 0, &device_view) ==
                PDJE_INPUT_RESULT_OK_V1);
        CHECK(device_view.struct_size == sizeof(device_view));
        const bool device_type_is_known =
            device_view.type == PDJE_INPUT_DEVICE_MOUSE_V1 ||
            device_view.type == PDJE_INPUT_DEVICE_KEYBOARD_V1 ||
            device_view.type == PDJE_INPUT_DEVICE_UNKNOWN_V1;
        CHECK(device_type_is_known);
    } else {
        CHECK(pdje_input_device_list_get_v1(device_list, 0, &device_view) ==
              PDJE_INPUT_RESULT_OUT_OF_RANGE_V1);
    }

    PDJE_InputDeviceViewV1 bad_device_view {};
    bad_device_view.struct_size = 1;
    CHECK(pdje_input_device_list_get_v1(device_list, 0, &bad_device_view) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);

    PDJE_MidiDeviceListHandleV1 *midi_list = nullptr;
    REQUIRE(pdje_input_list_midi_devices_v1(input, &midi_list) ==
            PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(midi_list != nullptr);

    const auto midi_count = pdje_input_midi_device_list_size_v1(midi_list);
    PDJE_MidiDeviceViewV1 midi_view {};
    midi_view.struct_size = sizeof(midi_view);
    if (midi_count > 0) {
        REQUIRE(pdje_input_midi_device_list_get_v1(midi_list, 0, &midi_view) ==
                PDJE_INPUT_RESULT_OK_V1);
        CHECK(midi_view.struct_size == sizeof(midi_view));
    } else {
        CHECK(pdje_input_midi_device_list_get_v1(midi_list, 0, &midi_view) ==
              PDJE_INPUT_RESULT_OUT_OF_RANGE_V1);
    }

    PDJE_MidiDeviceViewV1 bad_midi_view {};
    bad_midi_view.struct_size = 1;
    CHECK(pdje_input_midi_device_list_get_v1(midi_list, 0, &bad_midi_view) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);

    CHECK(pdje_input_config_v1(input, nullptr, nullptr, 0, nullptr, nullptr, 0) ==
          PDJE_INPUT_RESULT_OPERATION_FAILED_V1);

    if (device_count == 0) {
        const size_t bad_device_index = 0;
        CHECK(pdje_input_config_v1(input,
                                   device_list,
                                   &bad_device_index,
                                   1,
                                   midi_list,
                                   nullptr,
                                   0) == PDJE_INPUT_RESULT_OUT_OF_RANGE_V1);
    }

    if (midi_count == 0) {
        const size_t bad_midi_index = 0;
        CHECK(pdje_input_config_v1(input,
                                   device_list,
                                   nullptr,
                                   0,
                                   midi_list,
                                   &bad_midi_index,
                                   1) == PDJE_INPUT_RESULT_OUT_OF_RANGE_V1);
    }

    PDJE_InputSnapshotHandleV1 *snapshot = nullptr;
    REQUIRE(pdje_input_poll_snapshot_v1(input, &snapshot) ==
            PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(snapshot != nullptr);

    PDJE_InputSnapshotInfoV1 info {};
    info.struct_size = sizeof(info);
    REQUIRE(pdje_input_snapshot_describe_v1(snapshot, &info) ==
            PDJE_INPUT_RESULT_OK_V1);
    CHECK(info.input_event_count == pdje_input_snapshot_input_size_v1(snapshot));
    CHECK(info.midi_event_count == pdje_input_snapshot_midi_size_v1(snapshot));

    PDJE_InputSnapshotInfoV1 bad_info {};
    bad_info.struct_size = 1;
    CHECK(pdje_input_snapshot_describe_v1(snapshot, &bad_info) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);

    pdje_input_snapshot_destroy_v1(snapshot);
    pdje_input_midi_device_list_destroy_v1(midi_list);
    pdje_input_device_list_destroy_v1(device_list);

    REQUIRE(pdje_input_kill_v1(input) == PDJE_INPUT_RESULT_OK_V1);
    REQUIRE(pdje_input_get_state_v1(input, &state) == PDJE_INPUT_RESULT_OK_V1);
    CHECK(state == PDJE_INPUT_STATE_DEAD_V1);

    pdje_input_destroy_v1(input);
}

TEST_CASE("CPDJE input C ABI rejects null and mismatched outputs")
{
    CHECK(pdje_input_create_v1(nullptr) == PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_input_init_v1(nullptr, nullptr, nullptr, 0) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_input_kill_v1(nullptr) == PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_input_get_state_v1(nullptr, nullptr) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_input_get_backend_name_v1(nullptr, nullptr) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_input_poll_snapshot_v1(nullptr, nullptr) ==
          PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1);
}
