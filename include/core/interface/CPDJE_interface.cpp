#include "CPDJE_interface.h"

#include "PDJE_CAbi_Core_Private.hpp"
#include "PDJE_CAbi_Private.hpp"
#include "PDJE_interface.hpp"

#include <atomic>
#include <memory>
#include <string>
#include <vector>

struct PDJE_MusicListHandleV1 {
    MUS_VEC items;
};

struct PDJE_TrackListHandleV1 {
    TRACK_VEC items;
};

struct PDJE_PcmBufferHandleV1 {
    std::vector<float> samples;
};

namespace {

PDJE *
GetEngine(PDJE_EngineHandleV1 *handle) noexcept
{
    return handle == nullptr ? nullptr : static_cast<PDJE *>(handle->engine);
}

const PDJE *
GetEngine(const PDJE_EngineHandleV1 *handle) noexcept
{
    return handle == nullptr ? nullptr
                             : static_cast<const PDJE *>(handle->engine);
}

void
RefreshCoreLineCache(PDJE_EngineHandleV1 *handle)
{
    if (handle == nullptr) {
        return;
    }
    auto *engine = GetEngine(handle);
    if (engine == nullptr) {
        handle->now_cursor   = nullptr;
        handle->max_cursor   = nullptr;
        handle->pre_rendered = nullptr;
        handle->sync_data    = nullptr;
        return;
    }
    const auto raw_line  = engine->PullOutDataLine();
    handle->now_cursor   = raw_line.nowCursor;
    handle->max_cursor   = raw_line.maxCursor;
    handle->pre_rendered = raw_line.preRenderedData;
    handle->sync_data    = raw_line.syncD;
}

PDJE_BytesViewV1
MakeBytesView(const BIN &value) noexcept
{
    return PDJE_BytesViewV1{ value.empty() ? nullptr
                                           : reinterpret_cast<const uint8_t *>(
                                                 value.data()),
                             value.size() };
}

PLAY_MODE
ToCppTrackPlayMode(const PDJE_PlayModeV1 mode, bool &is_valid) noexcept
{
    switch (mode) {
    case PDJE_PLAY_MODE_FULL_PRE_RENDER_V1:
        is_valid = true;
        return PLAY_MODE::FULL_PRE_RENDER;
    case PDJE_PLAY_MODE_HYBRID_RENDER_V1:
        is_valid = true;
        return PLAY_MODE::HYBRID_RENDER;
    default:
        is_valid = false;
        return PLAY_MODE::FULL_PRE_RENDER;
    }
}

} // namespace

int PDJE_CALL
pdje_engine_create_v1(const char *root_dir, PDJE_EngineHandleV1 **out_engine)
{
    return PDJE_CABI::Guard(
        "pdje_engine_create_v1 failed",
        PDJE_RESULT_INTERNAL_ERROR_V1,
        [&]() -> int {
            if (!PDJE_CABI::PrepareHandleOutput(out_engine) ||
                root_dir == nullptr) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }

            auto engine    = std::make_unique<PDJE>(root_dir);
            auto handle    = std::make_unique<PDJE_EngineHandleV1>();
            handle->engine = engine.get();
            RefreshCoreLineCache(handle.get());
            engine.release();
            *out_engine = handle.release();
            return PDJE_RESULT_OK_V1;
        });
}

void PDJE_CALL
pdje_engine_destroy_v1(PDJE_EngineHandleV1 *engine)
{
    if (engine == nullptr) {
        return;
    }
    PDJE_CABI::GuardVoid("pdje_engine_destroy_v1 failed", [&]() {
        auto *engine_obj = GetEngine(engine);
        engine->engine   = nullptr;
        delete engine_obj;
        delete engine;
    });
}

int PDJE_CALL
pdje_engine_search_music_v1(PDJE_EngineHandleV1     *engine,
                            const char              *title,
                            const char              *composer,
                            double                   bpm,
                            PDJE_MusicListHandleV1 **out_list)
{
    return PDJE_CABI::Guard("pdje_engine_search_music_v1 failed",
                            PDJE_RESULT_INTERNAL_ERROR_V1,
                            [&]() -> int {
                                if (!PDJE_CABI::PrepareHandleOutput(out_list) ||
                                    engine == nullptr) {
                                    return PDJE_RESULT_INVALID_ARGUMENT_V1;
                                }

                                auto *engine_obj = GetEngine(engine);
                                if (engine_obj == nullptr) {
                                    return PDJE_RESULT_INTERNAL_ERROR_V1;
                                }

                                auto list =
                                    std::make_unique<PDJE_MusicListHandleV1>();
                                list->items = engine_obj->SearchMusic(
                                    PDJE_CABI::OptionalString(title),
                                    PDJE_CABI::OptionalString(composer),
                                    bpm);
                                *out_list = list.release();
                                return PDJE_RESULT_OK_V1;
                            });
}

size_t PDJE_CALL
pdje_music_list_size_v1(const PDJE_MusicListHandleV1 *list)
{
    return list != nullptr ? list->items.size() : 0;
}

int PDJE_CALL
pdje_music_list_get_v1(const PDJE_MusicListHandleV1 *list,
                       size_t                        index,
                       PDJE_MusicViewV1             *out_music)
{
    return PDJE_CABI::Guard(
        "pdje_music_list_get_v1 failed",
        PDJE_RESULT_INTERNAL_ERROR_V1,
        [&]() -> int {
            if (!PDJE_CABI::StructIsCompatible(out_music)) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_music);
            if (list == nullptr) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }
            if (index >= list->items.size()) {
                return PDJE_RESULT_OUT_OF_RANGE_V1;
            }

            const auto &item = list->items[index];
            out_music->title =
                PDJE_CABI::MakeStringView<PDJE_StringViewV1>(item.title);
            out_music->composer =
                PDJE_CABI::MakeStringView<PDJE_StringViewV1>(item.composer);
            out_music->music_path =
                PDJE_CABI::MakeStringView<PDJE_StringViewV1>(item.musicPath);
            out_music->bpm_binary = MakeBytesView(item.bpmBinary);
            out_music->bpm        = item.bpm;
            out_music->first_beat =
                PDJE_CABI::MakeStringView<PDJE_StringViewV1>(item.firstBeat);
            return PDJE_RESULT_OK_V1;
        });
}

void PDJE_CALL
pdje_music_list_destroy_v1(PDJE_MusicListHandleV1 *list)
{
    PDJE_CABI::GuardVoid("pdje_music_list_destroy_v1 failed",
                         [&]() { delete list; });
}

int PDJE_CALL
pdje_engine_search_track_v1(PDJE_EngineHandleV1     *engine,
                            const char              *title,
                            PDJE_TrackListHandleV1 **out_list)
{
    return PDJE_CABI::Guard("pdje_engine_search_track_v1 failed",
                            PDJE_RESULT_INTERNAL_ERROR_V1,
                            [&]() -> int {
                                if (!PDJE_CABI::PrepareHandleOutput(out_list) ||
                                    engine == nullptr) {
                                    return PDJE_RESULT_INVALID_ARGUMENT_V1;
                                }

                                auto *engine_obj = GetEngine(engine);
                                if (engine_obj == nullptr) {
                                    return PDJE_RESULT_INTERNAL_ERROR_V1;
                                }

                                auto list =
                                    std::make_unique<PDJE_TrackListHandleV1>();
                                list->items = engine_obj->SearchTrack(
                                    PDJE_CABI::OptionalString(title));
                                *out_list = list.release();
                                return PDJE_RESULT_OK_V1;
                            });
}

size_t PDJE_CALL
pdje_track_list_size_v1(const PDJE_TrackListHandleV1 *list)
{
    return list != nullptr ? list->items.size() : 0;
}

int PDJE_CALL
pdje_track_list_get_v1(const PDJE_TrackListHandleV1 *list,
                       size_t                        index,
                       PDJE_TrackViewV1             *out_track)
{
    return PDJE_CABI::Guard(
        "pdje_track_list_get_v1 failed",
        PDJE_RESULT_INTERNAL_ERROR_V1,
        [&]() -> int {
            if (!PDJE_CABI::StructIsCompatible(out_track)) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_track);
            if (list == nullptr) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }
            if (index >= list->items.size()) {
                return PDJE_RESULT_OUT_OF_RANGE_V1;
            }

            const auto &item = list->items[index];
            out_track->track_title =
                PDJE_CABI::MakeStringView<PDJE_StringViewV1>(item.trackTitle);
            out_track->mix_binary  = MakeBytesView(item.mixBinary);
            out_track->note_binary = MakeBytesView(item.noteBinary);
            out_track->cached_mix_list =
                PDJE_CABI::MakeStringView<PDJE_StringViewV1>(
                    item.cachedMixList);
            return PDJE_RESULT_OK_V1;
        });
}

void PDJE_CALL
pdje_track_list_destroy_v1(PDJE_TrackListHandleV1 *list)
{
    PDJE_CABI::GuardVoid("pdje_track_list_destroy_v1 failed",
                         [&]() { delete list; });
}

int PDJE_CALL
pdje_engine_init_player_from_track_v1(PDJE_EngineHandleV1          *engine,
                                      PDJE_PlayModeV1               mode,
                                      const PDJE_TrackListHandleV1 *tracks,
                                      size_t                        track_index,
                                      uint32_t frame_buffer_size)
{
    return PDJE_CABI::Guard(
        "pdje_engine_init_player_from_track_v1 failed",
        PDJE_RESULT_INTERNAL_ERROR_V1,
        [&]() -> int {
            if (engine == nullptr || tracks == nullptr) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }
            if (track_index >= tracks->items.size()) {
                return PDJE_RESULT_OUT_OF_RANGE_V1;
            }

            bool      is_valid_mode = false;
            PLAY_MODE cpp_mode      = ToCppTrackPlayMode(mode, is_valid_mode);
            if (!is_valid_mode) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }

            auto  selected_track = tracks->items[track_index];
            auto *engine_obj     = GetEngine(engine);
            if (engine_obj == nullptr) {
                return PDJE_RESULT_INTERNAL_ERROR_V1;
            }
            const bool ok = engine_obj->InitPlayer(
                cpp_mode, selected_track, frame_buffer_size);
            RefreshCoreLineCache(engine);
            return ok ? PDJE_RESULT_OK_V1 : PDJE_RESULT_INTERNAL_ERROR_V1;
        });
}

int PDJE_CALL
pdje_engine_init_player_manual_v1(PDJE_EngineHandleV1 *engine,
                                  uint32_t             frame_buffer_size)
{
    return PDJE_CABI::Guard(
        "pdje_engine_init_player_manual_v1 failed",
        PDJE_RESULT_INTERNAL_ERROR_V1,
        [&]() -> int {
            if (engine == nullptr) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }

            trackdata unused_track;
            auto     *engine_obj = GetEngine(engine);
            if (engine_obj == nullptr) {
                return PDJE_RESULT_INTERNAL_ERROR_V1;
            }
            const bool ok = engine_obj->InitPlayer(
                PLAY_MODE::FULL_MANUAL_RENDER, unused_track, frame_buffer_size);
            RefreshCoreLineCache(engine);
            return ok ? PDJE_RESULT_OK_V1 : PDJE_RESULT_INTERNAL_ERROR_V1;
        });
}

void PDJE_CALL
pdje_engine_reset_player_v1(PDJE_EngineHandleV1 *engine)
{
    if (engine == nullptr) {
        return;
    }
    PDJE_CABI::GuardVoid("pdje_engine_reset_player_v1 failed", [&]() {
        auto *engine_obj = GetEngine(engine);
        if (engine_obj != nullptr) {
            engine_obj->ResetPlayer();
        }
        RefreshCoreLineCache(engine);
    });
}

int PDJE_CALL
pdje_engine_init_editor_v1(PDJE_EngineHandleV1 *engine,
                           const char          *auth_name,
                           const char          *auth_email,
                           const char          *project_root)
{
    return PDJE_CABI::Guard(
        "pdje_engine_init_editor_v1 failed",
        PDJE_RESULT_INTERNAL_ERROR_V1,
        [&]() -> int {
            if (engine == nullptr || auth_name == nullptr ||
                auth_email == nullptr || project_root == nullptr) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }

            auto *engine_obj = GetEngine(engine);
            if (engine_obj == nullptr) {
                return PDJE_RESULT_INTERNAL_ERROR_V1;
            }
            const bool ok =
                engine_obj->InitEditor(auth_name, auth_email, project_root);
            return ok ? PDJE_RESULT_OK_V1 : PDJE_RESULT_INTERNAL_ERROR_V1;
        });
}

void PDJE_CALL
pdje_engine_close_editor_v1(PDJE_EngineHandleV1 *engine)
{
    if (engine == nullptr) {
        return;
    }
    PDJE_CABI::GuardVoid("pdje_engine_close_editor_v1 failed", [&]() {
        auto *engine_obj = GetEngine(engine);
        if (engine_obj != nullptr) {
            engine_obj->CloseEditor();
        }
    });
}

int PDJE_CALL
pdje_engine_get_pcm_from_music_v1(PDJE_EngineHandleV1          *engine,
                                  const PDJE_MusicListHandleV1 *musics,
                                  size_t                        music_index,
                                  PDJE_PcmBufferHandleV1      **out_pcm)
{
    return PDJE_CABI::Guard("pdje_engine_get_pcm_from_music_v1 failed",
                            PDJE_RESULT_INTERNAL_ERROR_V1,
                            [&]() -> int {
                                if (!PDJE_CABI::PrepareHandleOutput(out_pcm) ||
                                    engine == nullptr || musics == nullptr) {
                                    return PDJE_RESULT_INVALID_ARGUMENT_V1;
                                }
                                if (music_index >= musics->items.size()) {
                                    return PDJE_RESULT_OUT_OF_RANGE_V1;
                                }

                                auto *engine_obj = GetEngine(engine);
                                if (engine_obj == nullptr) {
                                    return PDJE_RESULT_INTERNAL_ERROR_V1;
                                }

                                auto pcm =
                                    std::make_unique<PDJE_PcmBufferHandleV1>();
                                pcm->samples = engine_obj->GetPCMFromMusData(
                                    musics->items[music_index]);
                                if (pcm->samples.empty()) {
                                    return PDJE_RESULT_INTERNAL_ERROR_V1;
                                }

                                *out_pcm = pcm.release();
                                return PDJE_RESULT_OK_V1;
                            });
}

size_t PDJE_CALL
pdje_pcm_buffer_size_v1(const PDJE_PcmBufferHandleV1 *pcm)
{
    return pcm != nullptr ? pcm->samples.size() : 0;
}

const float *PDJE_CALL
pdje_pcm_buffer_data_v1(const PDJE_PcmBufferHandleV1 *pcm)
{
    if (pcm == nullptr || pcm->samples.empty()) {
        return nullptr;
    }
    return pcm->samples.data();
}

void PDJE_CALL
pdje_pcm_buffer_destroy_v1(PDJE_PcmBufferHandleV1 *pcm)
{
    PDJE_CABI::GuardVoid("pdje_pcm_buffer_destroy_v1 failed",
                         [&]() { delete pcm; });
}

int PDJE_CALL
pdje_engine_pull_core_dataline_v1(const PDJE_EngineHandleV1   *engine,
                                  PDJE_CoreDataLineSnapshotV1 *out_line)
{
    return PDJE_CABI::Guard(
        "pdje_engine_pull_core_dataline_v1 failed",
        PDJE_RESULT_INTERNAL_ERROR_V1,
        [&]() -> int {
            if (!PDJE_CABI::StructIsCompatible(out_line)) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }
            PDJE_CABI::ResetVersionedOutput(out_line);
            if (engine == nullptr) {
                return PDJE_RESULT_INVALID_ARGUMENT_V1;
            }

            auto *engine_obj = GetEngine(engine);
            if (engine_obj == nullptr) {
                return PDJE_RESULT_INTERNAL_ERROR_V1;
            }
            RefreshCoreLineCache(const_cast<PDJE_EngineHandleV1 *>(engine));
            out_line->has_player = engine_obj->player != nullptr ? 1 : 0;

            if (engine->now_cursor != nullptr) {
                out_line->now_cursor = *engine->now_cursor;
            }
            if (engine->max_cursor != nullptr) {
                out_line->max_cursor = *engine->max_cursor;
            }
            if (engine->pre_rendered != nullptr) {
                out_line->pre_rendered_data     = engine->pre_rendered;
                out_line->has_pre_rendered_data = 1;
            }
            if (engine->sync_data != nullptr) {
                const auto sync =
                    static_cast<std::atomic<audioSyncData> *>(engine->sync_data)
                        ->load();
                out_line->has_sync             = 1;
                out_line->sync.consumed_frames = sync.consumed_frames;
                out_line->sync.pre_calculated_unused_frames =
                    sync.pre_calculated_unused_frames;
                out_line->sync.microsecond = sync.microsecond;
            }

            return PDJE_RESULT_OK_V1;
        });
}
