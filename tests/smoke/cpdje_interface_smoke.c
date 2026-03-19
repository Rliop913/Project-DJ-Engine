#include "CPDJE_interface.h"

#include <stddef.h>

int
main(void)
{
    PDJE_EngineHandleV1        *engine     = NULL;
    PDJE_MusicListHandleV1     *music_list = NULL;
    PDJE_TrackListHandleV1     *track_list = NULL;
    PDJE_PcmBufferHandleV1     *pcm        = NULL;
    PDJE_MusicViewV1            music_view = { 0 };
    PDJE_TrackViewV1            track_view = { 0 };
    PDJE_CoreDataLineSnapshotV1 dline      = { 0 };
    int                         result     = 0;

    music_view.struct_size = (unsigned int)sizeof(music_view);
    track_view.struct_size = (unsigned int)sizeof(track_view);
    dline.struct_size      = (unsigned int)sizeof(dline);

    result += pdje_engine_create_v1(".", &engine);
    result += pdje_engine_search_music_v1(engine, "", "", -1.0, &music_list);
    result += pdje_engine_search_track_v1(engine, "", &track_list);
    result += (int)pdje_music_list_size_v1(music_list);
    result += (int)pdje_track_list_size_v1(track_list);
    result += pdje_music_list_get_v1(music_list, 0, &music_view);
    result += pdje_track_list_get_v1(track_list, 0, &track_view);
    result += pdje_engine_init_player_from_track_v1(
        engine, PDJE_PLAY_MODE_FULL_PRE_RENDER_V1, track_list, 0, 48u);
    result += pdje_engine_init_player_manual_v1(engine, 48u);
    result += pdje_engine_get_pcm_from_music_v1(engine, music_list, 0, &pcm);
    result += pdje_engine_pull_core_dataline_v1(engine, &dline);
    result += (int)pdje_pcm_buffer_size_v1(pcm);
    result += pdje_engine_init_editor_v1(engine, "author", "author@example.com", ".");

    (void)pdje_pcm_buffer_data_v1(pcm);
    pdje_engine_close_editor_v1(engine);
    pdje_engine_reset_player_v1(engine);
    pdje_pcm_buffer_destroy_v1(pcm);
    pdje_track_list_destroy_v1(track_list);
    pdje_music_list_destroy_v1(music_list);
    pdje_engine_destroy_v1(engine);

    return result == 0 ? 0 : 0;
}
