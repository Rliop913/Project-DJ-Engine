#pragma once

#include "PDJE_EXPORT_SETTER.hpp"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PDJE_EngineHandleV1 PDJE_EngineHandleV1;
typedef struct PDJE_MusicListHandleV1 PDJE_MusicListHandleV1;
typedef struct PDJE_TrackListHandleV1 PDJE_TrackListHandleV1;
typedef struct PDJE_PcmBufferHandleV1 PDJE_PcmBufferHandleV1;

typedef enum PDJE_ResultV1 {
    PDJE_RESULT_OK_V1               = 0,
    PDJE_RESULT_INVALID_ARGUMENT_V1 = 1,
    PDJE_RESULT_OUT_OF_RANGE_V1     = 2,
    PDJE_RESULT_INTERNAL_ERROR_V1   = 3
} PDJE_ResultV1;

typedef enum PDJE_PlayModeV1 {
    PDJE_PLAY_MODE_FULL_PRE_RENDER_V1    = 0,
    PDJE_PLAY_MODE_HYBRID_RENDER_V1      = 1,
    PDJE_PLAY_MODE_FULL_MANUAL_RENDER_V1 = 2
} PDJE_PlayModeV1;

typedef struct PDJE_StringViewV1 {
    const char *data;
    size_t      size;
} PDJE_StringViewV1;

typedef struct PDJE_BytesViewV1 {
    const uint8_t *data;
    size_t         size;
} PDJE_BytesViewV1;

typedef struct PDJE_MusicViewV1 {
    uint32_t          struct_size;
    PDJE_StringViewV1 title;
    PDJE_StringViewV1 composer;
    PDJE_StringViewV1 music_path;
    PDJE_BytesViewV1  bpm_binary;
    double            bpm;
    PDJE_StringViewV1 first_beat;
} PDJE_MusicViewV1;

typedef struct PDJE_TrackViewV1 {
    uint32_t          struct_size;
    PDJE_StringViewV1 track_title;
    PDJE_BytesViewV1  mix_binary;
    PDJE_BytesViewV1  note_binary;
    PDJE_StringViewV1 cached_mix_list;
} PDJE_TrackViewV1;

typedef struct PDJE_AudioSyncSnapshotV1 {
    uint64_t consumed_frames;
    uint64_t pre_calculated_unused_frames;
    uint64_t microsecond;
} PDJE_AudioSyncSnapshotV1;

typedef struct PDJE_CoreDataLineSnapshotV1 {
    uint32_t                 struct_size;
    int                      has_player;
    uint64_t                 now_cursor;
    uint64_t                 max_cursor;
    const float             *pre_rendered_data;
    int                      has_pre_rendered_data;
    int                      has_sync;
    PDJE_AudioSyncSnapshotV1 sync;
} PDJE_CoreDataLineSnapshotV1;

PDJE_API int PDJE_CALL
pdje_engine_create_v1(const char *root_dir, PDJE_EngineHandleV1 **out_engine);

PDJE_API void PDJE_CALL
pdje_engine_destroy_v1(PDJE_EngineHandleV1 *engine);

PDJE_API int PDJE_CALL
pdje_engine_search_music_v1(PDJE_EngineHandleV1     *engine,
                            const char              *title,
                            const char              *composer,
                            double                   bpm,
                            PDJE_MusicListHandleV1 **out_list);

PDJE_API size_t PDJE_CALL
pdje_music_list_size_v1(const PDJE_MusicListHandleV1 *list);

PDJE_API int PDJE_CALL
pdje_music_list_get_v1(const PDJE_MusicListHandleV1 *list,
                       size_t                        index,
                       PDJE_MusicViewV1            *out_music);

PDJE_API void PDJE_CALL
pdje_music_list_destroy_v1(PDJE_MusicListHandleV1 *list);

PDJE_API int PDJE_CALL
pdje_engine_search_track_v1(PDJE_EngineHandleV1     *engine,
                            const char              *title,
                            PDJE_TrackListHandleV1 **out_list);

PDJE_API size_t PDJE_CALL
pdje_track_list_size_v1(const PDJE_TrackListHandleV1 *list);

PDJE_API int PDJE_CALL
pdje_track_list_get_v1(const PDJE_TrackListHandleV1 *list,
                       size_t                        index,
                       PDJE_TrackViewV1            *out_track);

PDJE_API void PDJE_CALL
pdje_track_list_destroy_v1(PDJE_TrackListHandleV1 *list);

PDJE_API int PDJE_CALL
pdje_engine_init_player_from_track_v1(PDJE_EngineHandleV1          *engine,
                                      PDJE_PlayModeV1               mode,
                                      const PDJE_TrackListHandleV1 *tracks,
                                      size_t                        track_index,
                                      uint32_t                      frame_buffer_size);

PDJE_API int PDJE_CALL
pdje_engine_init_player_manual_v1(PDJE_EngineHandleV1 *engine,
                                  uint32_t             frame_buffer_size);

PDJE_API void PDJE_CALL
pdje_engine_reset_player_v1(PDJE_EngineHandleV1 *engine);

PDJE_API int PDJE_CALL
pdje_engine_init_editor_v1(PDJE_EngineHandleV1 *engine,
                           const char          *auth_name,
                           const char          *auth_email,
                           const char          *project_root);

PDJE_API void PDJE_CALL
pdje_engine_close_editor_v1(PDJE_EngineHandleV1 *engine);

PDJE_API int PDJE_CALL
pdje_engine_get_pcm_from_music_v1(PDJE_EngineHandleV1          *engine,
                                  const PDJE_MusicListHandleV1 *musics,
                                  size_t                        music_index,
                                  PDJE_PcmBufferHandleV1      **out_pcm);

PDJE_API size_t PDJE_CALL
pdje_pcm_buffer_size_v1(const PDJE_PcmBufferHandleV1 *pcm);

PDJE_API const float *PDJE_CALL
pdje_pcm_buffer_data_v1(const PDJE_PcmBufferHandleV1 *pcm);

PDJE_API void PDJE_CALL
pdje_pcm_buffer_destroy_v1(PDJE_PcmBufferHandleV1 *pcm);

PDJE_API int PDJE_CALL
pdje_engine_pull_core_dataline_v1(const PDJE_EngineHandleV1   *engine,
                                  PDJE_CoreDataLineSnapshotV1 *out_line);

#ifdef __cplusplus
}
#endif
