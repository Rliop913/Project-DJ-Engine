#include "CPDJE_Input.h"

#include <stddef.h>

int
main(void)
{
    PDJE_InputStateV1        state       = PDJE_INPUT_STATE_DEAD_V1;
    PDJE_InputStringViewV1   backend     = { 0 };
    PDJE_InputDeviceViewV1   device      = { 0 };
    PDJE_MidiDeviceViewV1    midi_device = { 0 };
    PDJE_InputSnapshotInfoV1 info        = { 0 };
    PDJE_InputEventViewV1    input_event = { 0 };
    PDJE_MidiEventViewV1     midi_event  = { 0 };

    device.struct_size      = (uint32_t)sizeof(device);
    midi_device.struct_size = (uint32_t)sizeof(midi_device);
    info.struct_size        = (uint32_t)sizeof(info);
    input_event.struct_size = (uint32_t)sizeof(input_event);
    midi_event.struct_size  = (uint32_t)sizeof(midi_event);

    (void)pdje_input_create_v1(NULL);
    pdje_input_destroy_v1(NULL);
    (void)pdje_input_init_v1(NULL, NULL, NULL, 0);
    (void)pdje_input_kill_v1(NULL);
    (void)pdje_input_get_state_v1(NULL, &state);
    (void)pdje_input_get_backend_name_v1(NULL, &backend);
    (void)pdje_input_list_devices_v1(NULL, NULL);
    (void)pdje_input_device_list_size_v1(NULL);
    (void)pdje_input_device_list_get_v1(NULL, 0, &device);
    pdje_input_device_list_destroy_v1(NULL);
    (void)pdje_input_list_midi_devices_v1(NULL, NULL);
    (void)pdje_input_midi_device_list_size_v1(NULL);
    (void)pdje_input_midi_device_list_get_v1(NULL, 0, &midi_device);
    pdje_input_midi_device_list_destroy_v1(NULL);
    (void)pdje_input_config_v1(NULL, NULL, NULL, 0, NULL, NULL, 0);
    (void)pdje_input_run_v1(NULL);
    (void)pdje_input_poll_snapshot_v1(NULL, NULL);
    (void)pdje_input_snapshot_describe_v1(NULL, &info);
    (void)pdje_input_snapshot_input_size_v1(NULL);
    (void)pdje_input_snapshot_input_get_v1(NULL, 0, &input_event);
    (void)pdje_input_snapshot_midi_size_v1(NULL);
    (void)pdje_input_snapshot_midi_get_v1(NULL, 0, &midi_event);
    pdje_input_snapshot_destroy_v1(NULL);
    return 0;
}
