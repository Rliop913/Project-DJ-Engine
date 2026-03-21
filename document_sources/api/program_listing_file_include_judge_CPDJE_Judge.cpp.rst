
.. _program_listing_file_include_judge_CPDJE_Judge.cpp:

Program Listing for File CPDJE_Judge.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_CPDJE_Judge.cpp>` (``include/judge/CPDJE_Judge.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "CPDJE_Judge.h"
   
   #include "PDJE_CAbi_Core_Private.hpp"
   #include "PDJE_CAbi_Input_Private.hpp"
   #include "PDJE_Judge.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   #include <chrono>
   #include <exception>
   #include <limits>
   #include <string>
   #include <unordered_map>
   #include <vector>
   
   struct PDJE_JudgeHandleV1 {
       PDJE_JUDGE::JUDGE judge;
   
       PDJE_EngineHandleV1 *attached_engine = nullptr;
       PDJE_InputHandleV1  *attached_input  = nullptr;
   
       PDJE_JudgeUsedCallbackV1   used_callback    = nullptr;
       void                      *used_user_data   = nullptr;
       PDJE_JudgeMissedCallbackV1 missed_callback  = nullptr;
       void                      *missed_user_data = nullptr;
   
       std::chrono::milliseconds used_sleep   = std::chrono::milliseconds(100);
       std::chrono::milliseconds missed_sleep = std::chrono::milliseconds(200);
   
       bool running = false;
   };
   
   namespace {
   
   template <typename Fn> PDJE_JudgeResultV1
   GuardJudgeAbi(const char *context, Fn &&fn) noexcept
   {
       try {
           return fn();
       } catch (const std::exception &e) {
           critlog(context);
           critlog(e.what());
           return PDJE_JUDGE_RESULT_INTERNAL_ERROR_V1;
       } catch (...) {
           critlog(context);
           return PDJE_JUDGE_RESULT_INTERNAL_ERROR_V1;
       }
   }
   
   const char *
   OptionalString(const char *value) noexcept
   {
       return value != nullptr ? value : "";
   }
   
   PDJE_JudgeStringViewV1
   MakeStringView(const std::string &value) noexcept
   {
       if (value.empty()) {
           return {};
       }
       return PDJE_JudgeStringViewV1 { value.c_str(), value.size() };
   }
   
   PDJE_JudgeStartStatusV1
   ToCStartStatus(const PDJE_JUDGE::JUDGE_STATUS status) noexcept
   {
       switch (status) {
       case PDJE_JUDGE::JUDGE_STATUS::OK:
           return PDJE_JUDGE_START_STATUS_OK_V1;
       case PDJE_JUDGE::JUDGE_STATUS::CORE_LINE_IS_MISSING:
           return PDJE_JUDGE_START_STATUS_CORE_LINE_MISSING_V1;
       case PDJE_JUDGE::JUDGE_STATUS::INPUT_LINE_IS_MISSING:
           return PDJE_JUDGE_START_STATUS_INPUT_LINE_MISSING_V1;
       case PDJE_JUDGE::JUDGE_STATUS::EVENT_RULE_IS_EMPTY:
           return PDJE_JUDGE_START_STATUS_EVENT_RULE_EMPTY_V1;
       case PDJE_JUDGE::JUDGE_STATUS::INPUT_RULE_IS_EMPTY:
           return PDJE_JUDGE_START_STATUS_INPUT_RULE_EMPTY_V1;
       default:
           return PDJE_JUDGE_START_STATUS_NOTE_OBJECT_MISSING_V1;
       }
   }
   
   std::chrono::milliseconds
   ClampMilliseconds(const uint64_t value) noexcept
   {
       using rep = std::chrono::milliseconds::rep;
       const auto max_value = static_cast<uint64_t>(std::numeric_limits<rep>::max());
       const auto clamped   = value > max_value ? max_value : value;
       return std::chrono::milliseconds(static_cast<rep>(clamped));
   }
   
   void
   RefreshAttachedLines(PDJE_JudgeHandleV1 *judge) noexcept
   {
       judge->judge.inits.coreline.reset();
       judge->judge.inits.inputline.reset();
   
       if (judge->attached_engine != nullptr) {
           const auto *core_line = PDJE_CABI::BorrowCoreDataLine(judge->attached_engine);
           if (core_line != nullptr && core_line->sync_data != nullptr) {
               PDJE_CORE_DATA_LINE raw_line {};
               raw_line.nowCursor       = core_line->now_cursor;
               raw_line.maxCursor       = core_line->max_cursor;
               raw_line.preRenderedData = core_line->pre_rendered;
               raw_line.syncD = static_cast<std::atomic<audioSyncData> *>(core_line->sync_data);
               judge->judge.inits.coreline = raw_line;
           }
       }
   
       if (judge->attached_input != nullptr) {
           const auto *input_line = PDJE_CABI::BorrowInputDataLine(judge->attached_input);
           if (input_line != nullptr &&
               (input_line->input_arena != nullptr || input_line->midi_datas != nullptr)) {
               PDJE_INPUT_DATA_LINE raw_line {};
               raw_line.input_arena = static_cast<PDJE_IPC::PDJE_Input_Transfer *>(
                   input_line->input_arena);
               raw_line.midi_datas = static_cast<Atomic_Double_Buffer<PDJE_MIDI::MIDI_EV> *>(
                   input_line->midi_datas);
               judge->judge.inits.inputline = raw_line;
           }
       }
   }
   
   void
   ConfigureCallbacks(PDJE_JudgeHandleV1 *judge)
   {
       PDJE_JUDGE::Custom_Events events {};
       events.use_event_sleep_time  = judge->used_sleep;
       events.miss_event_sleep_time = judge->missed_sleep;
       events.used_event = [judge](uint64_t railid, bool pressed, bool is_late, uint64_t diff) {
           if (judge->used_callback == nullptr) {
               return;
           }
   
           const PDJE_JudgeUsedEventV1 event {
               railid,
               pressed ? 1 : 0,
               is_late ? 1 : 0,
               diff
           };
           judge->used_callback(&event, judge->used_user_data);
       };
       events.missed_event = [judge](std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC> missed) {
           if (judge->missed_callback == nullptr) {
               return;
           }
   
           size_t total_notes = 0;
           for (const auto &entry : missed) {
               total_notes += entry.second.size();
           }
   
           std::vector<PDJE_JudgeMissedNoteV1> views;
           views.reserve(total_notes);
           for (const auto &entry : missed) {
               for (const auto &note : entry.second) {
                   const auto microsecond = note.microsecond < 0
                                                ? 0ULL
                                                : static_cast<uint64_t>(note.microsecond);
                   views.push_back(PDJE_JudgeMissedNoteV1 {
                       entry.first,
                       MakeStringView(note.type),
                       note.detail,
                       MakeStringView(note.first),
                       MakeStringView(note.second),
                       MakeStringView(note.third),
                       microsecond,
                       note.used ? 1 : 0,
                       note.isDown ? 1 : 0
                   });
               }
           }
   
           judge->missed_callback(
               views.empty() ? nullptr : views.data(), views.size(), judge->missed_user_data);
       };
       judge->judge.inits.SetCustomEvents(events);
   }
   
   } // namespace
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_create_v1(PDJE_JudgeHandleV1 **out_judge)
   {
       return GuardJudgeAbi("pdje_judge_create_v1 failed", [&]() {
           if (out_judge == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           *out_judge = new PDJE_JudgeHandleV1();
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_judge_destroy_v1(PDJE_JudgeHandleV1 *judge)
   {
       if (judge != nullptr && judge->running) {
           pdje_judge_end_v1(judge);
       }
       delete judge;
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_attach_engine_v1(PDJE_JudgeHandleV1 *judge, PDJE_EngineHandleV1 *engine)
   {
       return GuardJudgeAbi("pdje_judge_attach_engine_v1 failed", [&]() {
           if (judge == nullptr || engine == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
           judge->attached_engine = engine;
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_attach_input_v1(PDJE_JudgeHandleV1 *judge, PDJE_InputHandleV1 *input)
   {
       return GuardJudgeAbi("pdje_judge_attach_input_v1 failed", [&]() {
           if (judge == nullptr || input == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
           judge->attached_input = input;
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_set_event_rule_v1(PDJE_JudgeHandleV1 *judge,
                                uint64_t            miss_range_microsecond,
                                uint64_t            use_range_microsecond)
   {
       return GuardJudgeAbi("pdje_judge_set_event_rule_v1 failed", [&]() {
           if (judge == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
   
           PDJE_JUDGE::EVENT_RULE event_rule {};
           event_rule.miss_range_microsecond = miss_range_microsecond;
           event_rule.use_range_microsecond  = use_range_microsecond;
           judge->judge.inits.SetEventRule(event_rule);
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_add_input_rail_v1(PDJE_JudgeHandleV1                 *judge,
                                const PDJE_InputDeviceListHandleV1 *devices,
                                size_t                              device_index,
                                uint16_t                            device_key_mask,
                                int64_t                             offset_microsecond,
                                uint64_t                            match_rail)
   {
       return GuardJudgeAbi("pdje_judge_add_input_rail_v1 failed", [&]() {
           if (judge == nullptr || devices == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
   
           const auto *device = PDJE_CABI::TryGetInputDevice(devices, device_index);
           if (device == nullptr) {
               return PDJE_JUDGE_RESULT_OUT_OF_RANGE_V1;
           }
   
           judge->judge.inits.SetRail(
               *device, device_key_mask, offset_microsecond, match_rail);
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_add_midi_rail_v1(PDJE_JudgeHandleV1                *judge,
                               const PDJE_MidiDeviceListHandleV1 *midi_devices,
                               size_t                             midi_index,
                               uint64_t                           match_rail,
                               uint8_t                            type,
                               uint8_t                            channel,
                               uint8_t                            position,
                               int64_t                            offset_microsecond)
   {
       return GuardJudgeAbi("pdje_judge_add_midi_rail_v1 failed", [&]() {
           if (judge == nullptr || midi_devices == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
   
           const auto *midi_device = PDJE_CABI::TryGetMidiDevice(midi_devices, midi_index);
           if (midi_device == nullptr) {
               return PDJE_JUDGE_RESULT_OUT_OF_RANGE_V1;
           }
   
           judge->judge.inits.SetRail(
               *midi_device, match_rail, type, channel, position, offset_microsecond);
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_add_note_object_v1(PDJE_JudgeHandleV1 *judge,
                                 const char         *note_type,
                                 uint16_t            note_detail,
                                 const char         *first_arg,
                                 const char         *second_arg,
                                 const char         *third_arg,
                                 uint64_t            y_axis,
                                 uint64_t            y_axis_2,
                                 uint64_t            rail_id)
   {
       return GuardJudgeAbi("pdje_judge_add_note_object_v1 failed", [&]() {
           if (judge == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
           if (!judge->judge.inits.raildb.GetMETA(rail_id).has_value()) {
               return PDJE_JUDGE_RESULT_OUT_OF_RANGE_V1;
           }
   
           judge->judge.inits.NoteObjectCollector(OptionalString(note_type),
                                                  note_detail,
                                                  OptionalString(first_arg),
                                                  OptionalString(second_arg),
                                                  OptionalString(third_arg),
                                                  y_axis,
                                                  y_axis_2,
                                                  rail_id);
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_set_used_callback_v1(PDJE_JudgeHandleV1      *judge,
                                   PDJE_JudgeUsedCallbackV1 callback,
                                   void                    *user_data)
   {
       return GuardJudgeAbi("pdje_judge_set_used_callback_v1 failed", [&]() {
           if (judge == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
   
           judge->used_callback  = callback;
           judge->used_user_data = user_data;
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_set_missed_callback_v1(PDJE_JudgeHandleV1        *judge,
                                     PDJE_JudgeMissedCallbackV1 callback,
                                     void                      *user_data)
   {
       return GuardJudgeAbi("pdje_judge_set_missed_callback_v1 failed", [&]() {
           if (judge == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
   
           judge->missed_callback  = callback;
           judge->missed_user_data = user_data;
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_set_callback_intervals_v1(PDJE_JudgeHandleV1 *judge,
                                        uint64_t            used_event_sleep_millisecond,
                                        uint64_t            missed_event_sleep_millisecond)
   {
       return GuardJudgeAbi("pdje_judge_set_callback_intervals_v1 failed", [&]() {
           if (judge == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
   
           judge->used_sleep   = ClampMilliseconds(used_event_sleep_millisecond);
           judge->missed_sleep = ClampMilliseconds(missed_event_sleep_millisecond);
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   PDJE_JudgeResultV1 PDJE_CALL
   pdje_judge_start_v1(PDJE_JudgeHandleV1      *judge,
                       PDJE_JudgeStartStatusV1 *out_status)
   {
       return GuardJudgeAbi("pdje_judge_start_v1 failed", [&]() {
           if (judge == nullptr || out_status == nullptr) {
               return PDJE_JUDGE_RESULT_INVALID_ARGUMENT_V1;
           }
           if (judge->running) {
               return PDJE_JUDGE_RESULT_INVALID_STATE_V1;
           }
   
           ConfigureCallbacks(judge);
           RefreshAttachedLines(judge);
   
           const auto start_status = judge->judge.Start();
           *out_status             = ToCStartStatus(start_status);
           judge->running          = start_status == PDJE_JUDGE::JUDGE_STATUS::OK;
           return PDJE_JUDGE_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_judge_end_v1(PDJE_JudgeHandleV1 *judge)
   {
       if (judge != nullptr && judge->running) {
           judge->judge.End();
           judge->running = false;
       }
   }
   
   
