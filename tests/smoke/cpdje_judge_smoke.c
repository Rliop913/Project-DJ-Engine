#include "CPDJE_Judge.h"

#include <stddef.h>

static void PDJE_CALL
smoke_used_callback(const PDJE_JudgeUsedEventV1 *event, void *user_data)
{
    (void)event;
    (void)user_data;
}

static void PDJE_CALL
smoke_missed_callback(const PDJE_JudgeMissedNoteV1 *notes,
                      size_t                        note_count,
                      void                         *user_data)
{
    (void)notes;
    (void)note_count;
    (void)user_data;
}

int
main(void)
{
    PDJE_JudgeStartStatusV1 status = PDJE_JUDGE_START_STATUS_OK_V1;

    (void)pdje_judge_create_v1(NULL);
    pdje_judge_destroy_v1(NULL);
    (void)pdje_judge_attach_engine_v1(NULL, NULL);
    (void)pdje_judge_attach_input_v1(NULL, NULL);
    (void)pdje_judge_set_event_rule_v1(NULL, 0, 0);
    (void)pdje_judge_add_input_rail_v1(NULL, NULL, 0, 0, 0, 0);
    (void)pdje_judge_add_midi_rail_v1(NULL, NULL, 0, 0, 0, 0, 0, 0);
    (void)pdje_judge_add_note_object_v1(
        NULL, NULL, 0, NULL, NULL, NULL, 0, 0, 0);
    (void)pdje_judge_set_used_callback_v1(NULL, smoke_used_callback, NULL);
    (void)pdje_judge_set_missed_callback_v1(NULL, smoke_missed_callback, NULL);
    (void)pdje_judge_set_callback_intervals_v1(NULL, 0, 0);
    (void)pdje_judge_start_v1(NULL, &status);
    pdje_judge_end_v1(NULL);
    return 0;
}
