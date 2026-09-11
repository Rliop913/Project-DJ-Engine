#include "CPDJE_Input.h"

#include "PDJE_CAbi_Input_Private.hpp"
#include "PDJE_Input.hpp"
#include "PDJE_Input_Log.hpp"
#include "PDJE_Input_StateLogic.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct PDJE_InputSnapshotHandleV1 {
    bool                            has_input_stream = false;
    bool                            has_midi_stream  = false;
    std::vector<PDJE_Input_Log>     input_events;
    std::vector<PDJE_MIDI::MIDI_EV> midi_events;
};

namespace {

PDJE_Input *
GetInput(PDJE_InputHandleV1 *handle) noexcept
{
    return handle == nullptr ? nullptr
                             : static_cast<PDJE_Input *>(handle->input);
}

const PDJE_Input *
GetInput(const PDJE_InputHandleV1 *handle) noexcept
{
    return handle == nullptr ? nullptr
                             : static_cast<const PDJE_Input *>(handle->input);
}

void
RefreshInputDataLineCache(PDJE_InputHandleV1 *handle)
{
    if (handle == nullptr) {
        return;
    }
    auto *input = GetInput(handle);
    if (input == nullptr) {
        handle->input_arena = nullptr;
        handle->midi_datas  = nullptr;
        return;
    }
    const auto line     = input->PullOutDataLine();
    handle->input_arena = line.input_arena;
    handle->midi_datas  = line.midi_datas;
}

PDJE_InputStateV1
ToCState(const PDJE_INPUT_STATE state) noexcept
{
    switch (state) {
    case PDJE_INPUT_STATE::DEVICE_CONFIG_STATE:
        return PDJE_INPUT_STATE_DEVICE_CONFIG_V1;
    case PDJE_INPUT_STATE::INPUT_LOOP_READY:
        return PDJE_INPUT_STATE_LOOP_READY_V1;
    case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING:
        return PDJE_INPUT_STATE_LOOP_RUNNING_V1;
    default:
        return PDJE_INPUT_STATE_DEAD_V1;
    }
}

bool
CanEnumerate(PDJE_InputHandleV1 *input)
{
    auto *input_obj = GetInput(input);
    return input_obj != nullptr &&
           input_obj->GetState() != PDJE_INPUT_STATE::DEAD;
}

template <typename HandleT, typename ItemT>
PDJE_InputResultV1
GatherSelectedItems(const HandleT      *list,
                    const std::size_t  *indices,
                    const std::size_t   index_count,
                    std::vector<ItemT> &out_items)
{
    out_items.clear();
    if (index_count == 0) {
        return PDJE_INPUT_RESULT_OK_V1;
    }
    if (list == nullptr || indices == nullptr) {
        return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
    }

    std::vector<std::size_t> seen;
    seen.reserve(index_count);
    out_items.reserve(index_count);
    for (std::size_t i = 0; i < index_count; ++i) {
        const auto idx = indices[i];
        if (idx >= list->items.size()) {
            return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
        }
        if (std::find(seen.begin(), seen.end(), idx) != seen.end()) {
            continue;
        }
        seen.push_back(idx);
        out_items.push_back(list->items[idx]);
    }
    return PDJE_INPUT_RESULT_OK_V1;
}

} // namespace

PDJE_InputResultV1 PDJE_CALL
pdje_input_create_v1(PDJE_InputHandleV1 **out_input)
{
    return PDJE_CABI::Guard(
        "pdje_input_create_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::PrepareHandleOutput(out_input)) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }

            auto input    = std::make_unique<PDJE_Input>();
            auto handle   = std::make_unique<PDJE_InputHandleV1>();
            handle->input = input.get();
            RefreshInputDataLineCache(handle.get());
            input.release();
            *out_input = handle.release();
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

void PDJE_CALL
pdje_input_destroy_v1(PDJE_InputHandleV1 *input)
{
    if (input == nullptr) {
        return;
    }
    PDJE_CABI::GuardVoid("pdje_input_destroy_v1 failed", [&]() {
        auto *input_obj = GetInput(input);
        if (input_obj != nullptr &&
            input_obj->GetState() != PDJE_INPUT_STATE::DEAD) {
            input_obj->Kill();
            if (input_obj->GetState() != PDJE_INPUT_STATE::DEAD) {
                critlog(
                    "pdje_input_destroy_v1 retained an active input handle");
                return;
            }
        }

        input->input = nullptr;
        delete input_obj;
        delete input;
    });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_init_v1(PDJE_InputHandleV1 *input,
                   void               *platform_ctx0,
                   void               *platform_ctx1,
                   int                 use_internal_window)
{
    return PDJE_CABI::Guard(
        "pdje_input_init_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            if (!PDJE_INPUT_STATE_LOGIC::CanInit(input_obj->GetState())) {
                return PDJE_INPUT_RESULT_INVALID_STATE_V1;
            }
            if (!input_obj->Init(
                    platform_ctx0, platform_ctx1, use_internal_window != 0)) {
                return PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
            }
            RefreshInputDataLineCache(input);
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_kill_v1(PDJE_InputHandleV1 *input)
{
    return PDJE_CABI::Guard(
        "pdje_input_kill_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            const auto ok = input_obj->Kill();
            RefreshInputDataLineCache(input);
            return ok ? PDJE_INPUT_RESULT_OK_V1
                      : PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
        });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_get_state_v1(const PDJE_InputHandleV1 *input,
                        PDJE_InputStateV1        *out_state)
{
    return PDJE_CABI::Guard(
        "pdje_input_get_state_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (out_state == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            *out_state = PDJE_INPUT_STATE_DEAD_V1;
            if (input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            auto *input_obj = GetInput(const_cast<PDJE_InputHandleV1 *>(input));
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            *out_state =
                ToCState(const_cast<PDJE_Input &>(*input_obj).GetState());
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_get_backend_name_v1(PDJE_InputHandleV1     *input,
                               PDJE_InputStringViewV1 *out_backend)
{
    return PDJE_CABI::Guard(
        "pdje_input_get_backend_name_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (out_backend == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            *out_backend = {};
            if (input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            input->backend_name_cache = input_obj->GetCurrentInputBackend();
            *out_backend = PDJE_CABI::MakeStringView<PDJE_InputStringViewV1>(
                input->backend_name_cache);
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_list_devices_v1(PDJE_InputHandleV1            *input,
                           PDJE_InputDeviceListHandleV1 **out_list)
{
    return PDJE_CABI::Guard(
        "pdje_input_list_devices_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::PrepareHandleOutput(out_list) || input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            if (!CanEnumerate(input)) {
                return PDJE_INPUT_RESULT_INVALID_STATE_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            auto list   = std::make_unique<PDJE_InputDeviceListHandleV1>();
            list->items = input_obj->GetDevs();
            *out_list   = list.release();
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

size_t PDJE_CALL
pdje_input_device_list_size_v1(const PDJE_InputDeviceListHandleV1 *list)
{
    return list != nullptr ? list->items.size() : 0;
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_device_list_get_v1(const PDJE_InputDeviceListHandleV1 *list,
                              size_t                              index,
                              PDJE_InputDeviceViewV1             *out_device)
{
    return PDJE_CABI::Guard(
        "pdje_input_device_list_get_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::StructIsCompatible(out_device)) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_device);
            if (list == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            if (index >= list->items.size()) {
                return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
            }
            const auto &item = list->items[index];
            out_device->type = PDJE_CABI::ToInputDeviceType(item.Type);
            out_device->name =
                PDJE_CABI::MakeStringView<PDJE_InputStringViewV1>(item.Name);
            out_device->device_specific_id =
                PDJE_CABI::MakeStringView<PDJE_InputStringViewV1>(
                    item.device_specific_id);
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

void PDJE_CALL
pdje_input_device_list_destroy_v1(PDJE_InputDeviceListHandleV1 *list)
{
    PDJE_CABI::GuardVoid("pdje_input_device_list_destroy_v1 failed",
                         [&]() { delete list; });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_list_midi_devices_v1(PDJE_InputHandleV1           *input,
                                PDJE_MidiDeviceListHandleV1 **out_list)
{
    return PDJE_CABI::Guard(
        "pdje_input_list_midi_devices_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::PrepareHandleOutput(out_list) || input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            if (!CanEnumerate(input)) {
                return PDJE_INPUT_RESULT_INVALID_STATE_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            auto list   = std::make_unique<PDJE_MidiDeviceListHandleV1>();
            list->items = input_obj->GetMIDIDevs();
            *out_list   = list.release();
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

size_t PDJE_CALL
pdje_input_midi_device_list_size_v1(const PDJE_MidiDeviceListHandleV1 *list)
{
    return list != nullptr ? list->items.size() : 0;
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_midi_device_list_get_v1(const PDJE_MidiDeviceListHandleV1 *list,
                                   size_t                             index,
                                   PDJE_MidiDeviceViewV1 *out_device)
{
    return PDJE_CABI::Guard(
        "pdje_input_midi_device_list_get_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::StructIsCompatible(out_device)) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_device);
            if (list == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            if (index >= list->items.size()) {
                return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
            }
            const auto &item = list->items[index];
            out_device->manufacturer =
                PDJE_CABI::MakeStringView<PDJE_InputStringViewV1>(
                    item.manufacturer);
            out_device->device_name =
                PDJE_CABI::MakeStringView<PDJE_InputStringViewV1>(
                    item.device_name);
            out_device->port_name =
                PDJE_CABI::MakeStringView<PDJE_InputStringViewV1>(
                    item.port_name);
            out_device->display_name =
                PDJE_CABI::MakeStringView<PDJE_InputStringViewV1>(
                    item.display_name);
            out_device->client_handle = item.client;
            out_device->port_handle   = item.port;
            out_device->port_type     = static_cast<uint8_t>(item.type);
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

void PDJE_CALL
pdje_input_midi_device_list_destroy_v1(PDJE_MidiDeviceListHandleV1 *list)
{
    PDJE_CABI::GuardVoid("pdje_input_midi_device_list_destroy_v1 failed",
                         [&]() { delete list; });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_config_v1(PDJE_InputHandleV1                 *input,
                     const PDJE_InputDeviceListHandleV1 *devices,
                     const std::size_t                  *device_indices,
                     std::size_t                         device_index_count,
                     const PDJE_MidiDeviceListHandleV1  *midi_devices,
                     const std::size_t                  *midi_indices,
                     std::size_t                         midi_index_count)
{
    return PDJE_CABI::Guard(
        "pdje_input_config_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            if (!PDJE_INPUT_STATE_LOGIC::CanConfig(input_obj->GetState())) {
                return PDJE_INPUT_RESULT_INVALID_STATE_V1;
            }

            std::vector<DeviceData> selected_devices;
            const auto              device_result = GatherSelectedItems(
                devices, device_indices, device_index_count, selected_devices);
            if (device_result != PDJE_INPUT_RESULT_OK_V1) {
                return device_result;
            }

            std::vector<libremidi::input_port> selected_midi_devices;
            const auto midi_result = GatherSelectedItems(midi_devices,
                                                         midi_indices,
                                                         midi_index_count,
                                                         selected_midi_devices);
            if (midi_result != PDJE_INPUT_RESULT_OK_V1) {
                return midi_result;
            }

            const auto ok =
                input_obj->Config(selected_devices, selected_midi_devices);
            RefreshInputDataLineCache(input);
            return ok ? PDJE_INPUT_RESULT_OK_V1
                      : PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
        });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_run_v1(PDJE_InputHandleV1 *input)
{
    return PDJE_CABI::Guard(
        "pdje_input_run_v1 failed", PDJE_INPUT_RESULT_INTERNAL_ERROR_V1, [&]() {
            if (input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }
            if (!PDJE_INPUT_STATE_LOGIC::CanRun(input_obj->GetState())) {
                return PDJE_INPUT_RESULT_INVALID_STATE_V1;
            }
            const auto ok = input_obj->Run();
            RefreshInputDataLineCache(input);
            return ok ? PDJE_INPUT_RESULT_OK_V1
                      : PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
        });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_poll_snapshot_v1(PDJE_InputHandleV1          *input,
                            PDJE_InputSnapshotHandleV1 **out_snapshot)
{
    return PDJE_CABI::Guard(
        "pdje_input_poll_snapshot_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::PrepareHandleOutput(out_snapshot) ||
                input == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            auto *input_obj = GetInput(input);
            if (input_obj == nullptr) {
                return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
            }

            auto snapshot = std::make_unique<PDJE_InputSnapshotHandleV1>();
            RefreshInputDataLineCache(input);
            snapshot->has_input_stream = input->input_arena != nullptr;
            snapshot->has_midi_stream  = input->midi_datas != nullptr;

            if (input->input_arena != nullptr) {
                auto *arena = static_cast<PDJE_IPC::PDJE_Input_Transfer *>(
                    input->input_arena);
                arena->Receive();
                snapshot->input_events = arena->datas;
            }
            if (input->midi_datas != nullptr) {
                auto *midi_buffer =
                    static_cast<Atomic_Double_Buffer<PDJE_MIDI::MIDI_EV> *>(
                        input->midi_datas);
                const auto *midi_events = midi_buffer->Get();
                if (midi_events != nullptr) {
                    snapshot->midi_events = *midi_events;
                }
            }

            *out_snapshot = snapshot.release();
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_snapshot_describe_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                PDJE_InputSnapshotInfoV1         *out_info)
{
    return PDJE_CABI::Guard(
        "pdje_input_snapshot_describe_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::StructIsCompatible(out_info)) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_info);
            if (snapshot == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            out_info->has_input_stream  = snapshot->has_input_stream ? 1 : 0;
            out_info->has_midi_stream   = snapshot->has_midi_stream ? 1 : 0;
            out_info->input_event_count = snapshot->input_events.size();
            out_info->midi_event_count  = snapshot->midi_events.size();
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

size_t PDJE_CALL
pdje_input_snapshot_input_size_v1(const PDJE_InputSnapshotHandleV1 *snapshot)
{
    return snapshot != nullptr ? snapshot->input_events.size() : 0;
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_snapshot_input_get_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                 size_t                            index,
                                 PDJE_InputEventViewV1            *out_event)
{
    return PDJE_CABI::Guard(
        "pdje_input_snapshot_input_get_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::StructIsCompatible(out_event)) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_event);
            if (snapshot == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            if (index >= snapshot->input_events.size()) {
                return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
            }

            PDJE_CABI::FillInputEventViewFromLog(snapshot->input_events[index],
                                                 out_event);
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

size_t PDJE_CALL
pdje_input_snapshot_midi_size_v1(const PDJE_InputSnapshotHandleV1 *snapshot)
{
    return snapshot != nullptr ? snapshot->midi_events.size() : 0;
}

PDJE_InputResultV1 PDJE_CALL
pdje_input_snapshot_midi_get_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                size_t                            index,
                                PDJE_MidiEventViewV1             *out_event)
{
    return PDJE_CABI::Guard(
        "pdje_input_snapshot_midi_get_v1 failed",
        PDJE_INPUT_RESULT_INTERNAL_ERROR_V1,
        [&]() {
            if (!PDJE_CABI::StructIsCompatible(out_event)) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_event);
            if (snapshot == nullptr) {
                return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
            }
            if (index >= snapshot->midi_events.size()) {
                return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
            }

            const auto &item        = snapshot->midi_events[index];
            out_event->type         = item.type;
            out_event->channel      = item.ch;
            out_event->position     = item.pos;
            out_event->value        = item.value;
            out_event->highres_time = item.highres_time;
            out_event->port_name =
                PDJE_CABI::MakeCountedStringView<PDJE_InputStringViewV1>(
                    item.port_name,
                    std::min<std::size_t>(item.port_name_len,
                                          sizeof(item.port_name)));
            return PDJE_INPUT_RESULT_OK_V1;
        });
}

void PDJE_CALL
pdje_input_snapshot_destroy_v1(PDJE_InputSnapshotHandleV1 *snapshot)
{
    PDJE_CABI::GuardVoid("pdje_input_snapshot_destroy_v1 failed",
                         [&]() { delete snapshot; });
}
