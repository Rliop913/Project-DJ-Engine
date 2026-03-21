
.. _program_listing_file_include_core_interface_CPDJE_interface.cpp:

Program Listing for File CPDJE_interface.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_interface_CPDJE_interface.cpp>` (``include/core/interface/CPDJE_interface.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "CPDJE_interface.h"
   
   #include "PDJE_CAbi_Core_Private.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "PDJE_interface.hpp"
   
   #include <atomic>
   #include <exception>
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
       return handle == nullptr ? nullptr : static_cast<const PDJE *>(handle->engine);
   }
   
   void
   RefreshCoreLineCache(PDJE_EngineHandleV1 *handle) noexcept
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
       const auto raw_line = engine->PullOutDataLine();
       handle->now_cursor   = raw_line.nowCursor;
       handle->max_cursor   = raw_line.maxCursor;
       handle->pre_rendered = raw_line.preRenderedData;
       handle->sync_data    = raw_line.syncD;
   }
   
   template <typename Fn> int
   GuardCAbi(const char *context, Fn &&fn) noexcept
   {
       try {
           return fn();
       } catch (const std::exception &e) {
           critlog(context);
           critlog(e.what());
           return PDJE_RESULT_INTERNAL_ERROR_V1;
       } catch (...) {
           critlog(context);
           return PDJE_RESULT_INTERNAL_ERROR_V1;
       }
   }
   
   template <typename T> bool
   StructIsCompatible(const T *value) noexcept
   {
       if (value == nullptr) {
           return false;
       }
       return value->struct_size == 0 || value->struct_size >= sizeof(T);
   }
   
   const char *
   OptionalString(const char *value) noexcept
   {
       return value != nullptr ? value : "";
   }
   
   PDJE_StringViewV1
   MakeStringView(const std::string &value) noexcept
   {
       return PDJE_StringViewV1{ value.c_str(), value.size() };
   }
   
   PDJE_BytesViewV1
   MakeBytesView(const BIN &value) noexcept
   {
       return PDJE_BytesViewV1{
           value.empty() ? nullptr : reinterpret_cast<const uint8_t *>(value.data()),
           value.size()
       };
   }
   
   void
   ResetMusicView(PDJE_MusicViewV1 *out_music) noexcept
   {
       if (out_music == nullptr) {
           return;
       }
       const auto struct_size = out_music->struct_size;
       *out_music             = {};
       out_music->struct_size = struct_size != 0 ? struct_size : sizeof(*out_music);
   }
   
   void
   ResetTrackView(PDJE_TrackViewV1 *out_track) noexcept
   {
       if (out_track == nullptr) {
           return;
       }
       const auto struct_size = out_track->struct_size;
       *out_track             = {};
       out_track->struct_size = struct_size != 0 ? struct_size : sizeof(*out_track);
   }
   
   void
   ResetCoreDataLine(PDJE_CoreDataLineSnapshotV1 *out_line) noexcept
   {
       if (out_line == nullptr) {
           return;
       }
       const auto struct_size = out_line->struct_size;
       *out_line              = {};
       out_line->struct_size  = struct_size != 0 ? struct_size : sizeof(*out_line);
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
       return GuardCAbi("pdje_engine_create_v1 failed", [&]() -> int {
           if (root_dir == nullptr || out_engine == nullptr) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *handle  = new PDJE_EngineHandleV1();
           handle->engine = new PDJE(root_dir);
           RefreshCoreLineCache(handle);
           *out_engine = handle;
           return PDJE_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_engine_destroy_v1(PDJE_EngineHandleV1 *engine)
   {
       delete GetEngine(engine);
       delete engine;
   }
   
   int PDJE_CALL
   pdje_engine_search_music_v1(PDJE_EngineHandleV1     *engine,
                               const char              *title,
                               const char              *composer,
                               double                   bpm,
                               PDJE_MusicListHandleV1 **out_list)
   {
       return GuardCAbi("pdje_engine_search_music_v1 failed", [&]() -> int {
           if (engine == nullptr || out_list == nullptr) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
   
           auto *list = new PDJE_MusicListHandleV1();
           auto *engine_obj = GetEngine(engine);
           if (engine_obj == nullptr) {
               return PDJE_RESULT_INTERNAL_ERROR_V1;
           }
           list->items = engine_obj->SearchMusic(OptionalString(title),
                                                 OptionalString(composer),
                                                 bpm);
           *out_list = list;
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
                          PDJE_MusicViewV1            *out_music)
   {
       return GuardCAbi("pdje_music_list_get_v1 failed", [&]() -> int {
           if (list == nullptr || !StructIsCompatible(out_music)) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (index >= list->items.size()) {
               return PDJE_RESULT_OUT_OF_RANGE_V1;
           }
   
           ResetMusicView(out_music);
           const auto &item       = list->items[index];
           out_music->title       = MakeStringView(item.title);
           out_music->composer    = MakeStringView(item.composer);
           out_music->music_path  = MakeStringView(item.musicPath);
           out_music->bpm_binary  = MakeBytesView(item.bpmBinary);
           out_music->bpm         = item.bpm;
           out_music->first_beat  = MakeStringView(item.firstBeat);
           out_music->struct_size = sizeof(*out_music);
           return PDJE_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_music_list_destroy_v1(PDJE_MusicListHandleV1 *list)
   {
       delete list;
   }
   
   int PDJE_CALL
   pdje_engine_search_track_v1(PDJE_EngineHandleV1     *engine,
                               const char              *title,
                               PDJE_TrackListHandleV1 **out_list)
   {
       return GuardCAbi("pdje_engine_search_track_v1 failed", [&]() -> int {
           if (engine == nullptr || out_list == nullptr) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
   
           auto *list = new PDJE_TrackListHandleV1();
           auto *engine_obj = GetEngine(engine);
           if (engine_obj == nullptr) {
               return PDJE_RESULT_INTERNAL_ERROR_V1;
           }
           list->items = engine_obj->SearchTrack(OptionalString(title));
           *out_list   = list;
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
                          PDJE_TrackViewV1            *out_track)
   {
       return GuardCAbi("pdje_track_list_get_v1 failed", [&]() -> int {
           if (list == nullptr || !StructIsCompatible(out_track)) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (index >= list->items.size()) {
               return PDJE_RESULT_OUT_OF_RANGE_V1;
           }
   
           ResetTrackView(out_track);
           const auto &item           = list->items[index];
           out_track->track_title     = MakeStringView(item.trackTitle);
           out_track->mix_binary      = MakeBytesView(item.mixBinary);
           out_track->note_binary     = MakeBytesView(item.noteBinary);
           out_track->cached_mix_list = MakeStringView(item.cachedMixList);
           out_track->struct_size     = sizeof(*out_track);
           return PDJE_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_track_list_destroy_v1(PDJE_TrackListHandleV1 *list)
   {
       delete list;
   }
   
   int PDJE_CALL
   pdje_engine_init_player_from_track_v1(PDJE_EngineHandleV1          *engine,
                                         PDJE_PlayModeV1               mode,
                                         const PDJE_TrackListHandleV1 *tracks,
                                         size_t                        track_index,
                                         uint32_t                      frame_buffer_size)
   {
       return GuardCAbi("pdje_engine_init_player_from_track_v1 failed", [&]() -> int {
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
   
           auto selected_track = tracks->items[track_index];
           auto *engine_obj = GetEngine(engine);
           if (engine_obj == nullptr) {
               return PDJE_RESULT_INTERNAL_ERROR_V1;
           }
           const bool ok =
               engine_obj->InitPlayer(cpp_mode, selected_track, frame_buffer_size);
           RefreshCoreLineCache(engine);
           return ok ? PDJE_RESULT_OK_V1 : PDJE_RESULT_INTERNAL_ERROR_V1;
       });
   }
   
   int PDJE_CALL
   pdje_engine_init_player_manual_v1(PDJE_EngineHandleV1 *engine,
                                     uint32_t             frame_buffer_size)
   {
       return GuardCAbi("pdje_engine_init_player_manual_v1 failed", [&]() -> int {
           if (engine == nullptr) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
   
           trackdata unused_track;
           auto *engine_obj = GetEngine(engine);
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
       if (engine != nullptr) {
           auto *engine_obj = GetEngine(engine);
           if (engine_obj != nullptr) {
               engine_obj->ResetPlayer();
           }
           RefreshCoreLineCache(engine);
       }
   }
   
   int PDJE_CALL
   pdje_engine_init_editor_v1(PDJE_EngineHandleV1 *engine,
                              const char          *auth_name,
                              const char          *auth_email,
                              const char          *project_root)
   {
       return GuardCAbi("pdje_engine_init_editor_v1 failed", [&]() -> int {
           if (engine == nullptr || auth_name == nullptr || auth_email == nullptr ||
               project_root == nullptr) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
   
           auto *engine_obj = GetEngine(engine);
           if (engine_obj == nullptr) {
               return PDJE_RESULT_INTERNAL_ERROR_V1;
           }
           const bool ok = engine_obj->InitEditor(auth_name, auth_email, project_root);
           RefreshCoreLineCache(engine);
           return ok ? PDJE_RESULT_OK_V1 : PDJE_RESULT_INTERNAL_ERROR_V1;
       });
   }
   
   void PDJE_CALL
   pdje_engine_close_editor_v1(PDJE_EngineHandleV1 *engine)
   {
       if (engine != nullptr) {
           auto *engine_obj = GetEngine(engine);
           if (engine_obj != nullptr) {
               engine_obj->CloseEditor();
           }
           RefreshCoreLineCache(engine);
       }
   }
   
   int PDJE_CALL
   pdje_engine_get_pcm_from_music_v1(PDJE_EngineHandleV1          *engine,
                                     const PDJE_MusicListHandleV1 *musics,
                                     size_t                        music_index,
                                     PDJE_PcmBufferHandleV1      **out_pcm)
   {
       return GuardCAbi("pdje_engine_get_pcm_from_music_v1 failed", [&]() -> int {
           if (engine == nullptr || musics == nullptr || out_pcm == nullptr) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (music_index >= musics->items.size()) {
               return PDJE_RESULT_OUT_OF_RANGE_V1;
           }
   
           auto *pcm = new PDJE_PcmBufferHandleV1();
           auto *engine_obj = GetEngine(engine);
           if (engine_obj == nullptr) {
               delete pcm;
               return PDJE_RESULT_INTERNAL_ERROR_V1;
           }
           pcm->samples = engine_obj->GetPCMFromMusData(musics->items[music_index]);
           if (pcm->samples.empty()) {
               delete pcm;
               return PDJE_RESULT_INTERNAL_ERROR_V1;
           }
   
           *out_pcm = pcm;
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
       delete pcm;
   }
   
   int PDJE_CALL
   pdje_engine_pull_core_dataline_v1(const PDJE_EngineHandleV1   *engine,
                                     PDJE_CoreDataLineSnapshotV1 *out_line)
   {
       return GuardCAbi("pdje_engine_pull_core_dataline_v1 failed", [&]() -> int {
           if (engine == nullptr || !StructIsCompatible(out_line)) {
               return PDJE_RESULT_INVALID_ARGUMENT_V1;
           }
   
           ResetCoreDataLine(out_line);
           out_line->struct_size = sizeof(*out_line);
   
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
               const auto sync = static_cast<std::atomic<audioSyncData> *>(engine->sync_data)->load();
               out_line->has_sync = 1;
               out_line->sync.consumed_frames = sync.consumed_frames;
               out_line->sync.pre_calculated_unused_frames =
                   sync.pre_calculated_unused_frames;
               out_line->sync.microsecond = sync.microsecond;
           }
   
           return PDJE_RESULT_OK_V1;
       });
   }
   
   
   
