#include "CPDJE_interface.h"

#include <stddef.h>

int
main(void)
{
    PDJE_MusicViewV1            music_view = { 0 };
    PDJE_TrackViewV1            track_view = { 0 };
    PDJE_CoreDataLineSnapshotV1 data_line  = { 0 };

    music_view.struct_size = (uint32_t)sizeof(music_view);
    track_view.struct_size = (uint32_t)sizeof(track_view);
    data_line.struct_size  = (uint32_t)sizeof(data_line);

    (void)pdje_engine_create_v1(NULL, NULL);
    pdje_engine_destroy_v1(NULL);
    (void)pdje_engine_search_music_v1(NULL, NULL, NULL, -1.0, NULL);
    (void)pdje_music_list_size_v1(NULL);
    (void)pdje_music_list_get_v1(NULL, 0, &music_view);
    pdje_music_list_destroy_v1(NULL);
    (void)pdje_engine_search_track_v1(NULL, NULL, NULL);
    (void)pdje_track_list_size_v1(NULL);
    (void)pdje_track_list_get_v1(NULL, 0, &track_view);
    pdje_track_list_destroy_v1(NULL);
    (void)pdje_engine_init_player_from_track_v1(
        NULL, PDJE_PLAY_MODE_FULL_PRE_RENDER_V1, NULL, 0, 0);
    (void)pdje_engine_init_player_manual_v1(NULL, 0);
    pdje_engine_reset_player_v1(NULL);
    (void)pdje_engine_init_editor_v1(NULL, NULL, NULL, NULL);
    pdje_engine_close_editor_v1(NULL);
    (void)pdje_engine_get_pcm_from_music_v1(NULL, NULL, 0, NULL);
    (void)pdje_pcm_buffer_size_v1(NULL);
    (void)pdje_pcm_buffer_data_v1(NULL);
    pdje_pcm_buffer_destroy_v1(NULL);
    (void)pdje_engine_pull_core_dataline_v1(NULL, &data_line);
    return 0;
}
