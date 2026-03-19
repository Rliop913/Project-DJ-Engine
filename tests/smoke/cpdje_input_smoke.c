#include "CPDJE_Input.h"

#include <stddef.h>

int
main(void)
{
    PDJE_InputHandleV1           *input       = NULL;
    PDJE_InputDeviceListHandleV1 *device_list = NULL;
    PDJE_MidiDeviceListHandleV1  *midi_list   = NULL;
    PDJE_InputSnapshotHandleV1   *snapshot    = NULL;
    PDJE_InputStateV1             state       = PDJE_INPUT_STATE_DEAD_V1;
    PDJE_InputStringViewV1        backend     = { 0 };
    PDJE_InputDeviceViewV1        device_view = { 0 };
    PDJE_MidiDeviceViewV1         midi_view   = { 0 };
    PDJE_InputSnapshotInfoV1      info        = { 0 };
    PDJE_InputEventViewV1         input_event = { 0 };
    PDJE_MidiEventViewV1          midi_event  = { 0 };

    device_view.struct_size = (unsigned int)sizeof(device_view);
    midi_view.struct_size   = (unsigned int)sizeof(midi_view);
    info.struct_size        = (unsigned int)sizeof(info);
    input_event.struct_size = (unsigned int)sizeof(input_event);
    midi_event.struct_size  = (unsigned int)sizeof(midi_event);

    (void)pdje_input_create_v1(&input);
    (void)pdje_input_get_state_v1(input, &state);
    (void)pdje_input_get_backend_name_v1(input, &backend);
    (void)pdje_input_init_v1(input, NULL, NULL, 0);
    (void)pdje_input_list_devices_v1(input, &device_list);
    (void)pdje_input_list_midi_devices_v1(input, &midi_list);
    (void)pdje_input_device_list_size_v1(device_list);
    (void)pdje_input_midi_device_list_size_v1(midi_list);
    (void)pdje_input_device_list_get_v1(device_list, 0u, &device_view);
    (void)pdje_input_midi_device_list_get_v1(midi_list, 0u, &midi_view);
    (void)pdje_input_config_v1(input, device_list, NULL, 0u, midi_list, NULL, 0u);
    (void)pdje_input_run_v1(input);
    (void)pdje_input_poll_snapshot_v1(input, &snapshot);
    (void)pdje_input_snapshot_describe_v1(snapshot, &info);
    (void)pdje_input_snapshot_input_size_v1(snapshot);
    (void)pdje_input_snapshot_midi_size_v1(snapshot);
    (void)pdje_input_snapshot_input_get_v1(snapshot, 0u, &input_event);
    (void)pdje_input_snapshot_midi_get_v1(snapshot, 0u, &midi_event);

    pdje_input_snapshot_destroy_v1(snapshot);
    pdje_input_midi_device_list_destroy_v1(midi_list);
    pdje_input_device_list_destroy_v1(device_list);
    (void)pdje_input_kill_v1(input);
    pdje_input_destroy_v1(input);
    return 0;
}
