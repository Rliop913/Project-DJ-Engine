#pragma once

#include "CPDJE_Input.h"
#include "CPDJE_interface.h"
#include "PDJE_EXPORT_SETTER.hpp"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PDJE_JudgeHandleV1 PDJE_JudgeHandleV1;

typedef enum PDJE_JudgeResultV1 {
    PDJE_JUDGE_RESULT_OK_V1               = 0,
    PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1 = 1,
    PDJE_JUDGE_RESULT_INVALID_STATE_V1    = 2,
    PDJE_JUDGE_RESULT_OUT_OF_RANGE_V1     = 3,
    PDJE_JUDGE_RESULT_INTERNAL_ERROR_V1   = 4
} PDJE_JudgeResultV1;

typedef enum PDJE_JudgeStartStatusV1 {
    PDJE_JUDGE_START_STATUS_OK_V1                  = 0,
    PDJE_JUDGE_START_STATUS_CORE_LINE_MISSING_V1   = 1,
    PDJE_JUDGE_START_STATUS_INPUT_LINE_MISSING_V1  = 2,
    PDJE_JUDGE_START_STATUS_EVENT_RULE_EMPTY_V1    = 3,
    PDJE_JUDGE_START_STATUS_INPUT_RULE_EMPTY_V1    = 4,
    PDJE_JUDGE_START_STATUS_NOTE_OBJECT_MISSING_V1 = 5
} PDJE_JudgeStartStatusV1;

typedef struct PDJE_JudgeStringViewV1 {
    const char *data;
    size_t      size;
} PDJE_JudgeStringViewV1;

typedef struct PDJE_JudgeUsedEventV1 {
    uint64_t rail_id;
    int      pressed;
    int      is_late;
    uint64_t diff_microsecond;
} PDJE_JudgeUsedEventV1;

typedef struct PDJE_JudgeMissedNoteV1 {
    uint64_t               rail_id;
    PDJE_JudgeStringViewV1 type;
    uint16_t               detail;
    PDJE_JudgeStringViewV1 first;
    PDJE_JudgeStringViewV1 second;
    PDJE_JudgeStringViewV1 third;
    uint64_t               microsecond;
    int                    used;
    int                    is_down;
} PDJE_JudgeMissedNoteV1;

typedef void(PDJE_CALL *PDJE_JudgeUsedCallbackV1)(
    const PDJE_JudgeUsedEventV1 *event,
    void                        *user_data);
typedef void(PDJE_CALL *PDJE_JudgeMissedCallbackV1)(
    const PDJE_JudgeMissedNoteV1 *notes,
    size_t                        note_count,
    void                         *user_data);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_create_v1(PDJE_JudgeHandleV1 **out_judge);

PDJE_API void PDJE_CALL
pdje_judge_destroy_v1(PDJE_JudgeHandleV1 *judge);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_attach_engine_v1(PDJE_JudgeHandleV1 *judge,
                            PDJE_EngineHandleV1 *engine);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_attach_input_v1(PDJE_JudgeHandleV1 *judge, PDJE_InputHandleV1 *input);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_set_event_rule_v1(PDJE_JudgeHandleV1 *judge,
                             uint64_t            miss_range_microsecond,
                             uint64_t            use_range_microsecond);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_add_input_rail_v1(PDJE_JudgeHandleV1                 *judge,
                             const PDJE_InputDeviceListHandleV1 *devices,
                             size_t                              device_index,
                             uint16_t                            device_key_mask,
                             int64_t                             offset_microsecond,
                             uint64_t                            match_rail);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_add_midi_rail_v1(PDJE_JudgeHandleV1                *judge,
                            const PDJE_MidiDeviceListHandleV1 *midi_devices,
                            size_t                             midi_index,
                            uint64_t                           match_rail,
                            uint8_t                            type,
                            uint8_t                            channel,
                            uint8_t                            position,
                            int64_t                            offset_microsecond);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_add_note_object_v1(PDJE_JudgeHandleV1 *judge,
                              const char         *note_type,
                              uint16_t            note_detail,
                              const char         *first_arg,
                              const char         *second_arg,
                              const char         *third_arg,
                              uint64_t            y_axis,
                              uint64_t            y_axis_2,
                              uint64_t            rail_id);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_set_used_callback_v1(PDJE_JudgeHandleV1      *judge,
                                PDJE_JudgeUsedCallbackV1 callback,
                                void                    *user_data);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_set_missed_callback_v1(PDJE_JudgeHandleV1        *judge,
                                  PDJE_JudgeMissedCallbackV1 callback,
                                  void                      *user_data);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_set_callback_intervals_v1(
    PDJE_JudgeHandleV1 *judge,
    uint64_t            used_event_sleep_millisecond,
    uint64_t            missed_event_sleep_millisecond);

PDJE_API PDJE_JudgeResultV1 PDJE_CALL
pdje_judge_start_v1(PDJE_JudgeHandleV1      *judge,
                    PDJE_JudgeStartStatusV1 *out_status);

PDJE_API void PDJE_CALL
pdje_judge_end_v1(PDJE_JudgeHandleV1 *judge);

#ifdef __cplusplus
}
#endif
