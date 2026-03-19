#include "CPDJE_Judge.h"

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
    PDJE_JudgeHandleV1      *judge  = 0;
    PDJE_JudgeStartStatusV1  status = PDJE_JUDGE_START_STATUS_OK_V1;

    pdje_judge_create_v1(&judge);
    pdje_judge_attach_engine_v1(judge, 0);
    pdje_judge_attach_input_v1(judge, 0);
    pdje_judge_set_event_rule_v1(judge, 1000, 500);
    pdje_judge_add_input_rail_v1(judge, 0, 0, PDJE_MOUSE_L_BTN_DOWN, 0, 7);
    pdje_judge_add_midi_rail_v1(judge, 0, 0, 7, 1, 0, 0, 0);
    pdje_judge_add_note_object_v1(judge, "TAP", 0, "", "", "", 0, 0, 7);
    pdje_judge_set_used_callback_v1(judge, smoke_used_callback, 0);
    pdje_judge_set_missed_callback_v1(judge, smoke_missed_callback, 0);
    pdje_judge_set_callback_intervals_v1(judge, 10, 10);
    pdje_judge_start_v1(judge, &status);
    pdje_judge_end_v1(judge);
    pdje_judge_destroy_v1(judge);
    return 0;
}
