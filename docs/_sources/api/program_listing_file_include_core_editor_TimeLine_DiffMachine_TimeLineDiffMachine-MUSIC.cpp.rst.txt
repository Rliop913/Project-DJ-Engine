
.. _program_listing_file_include_core_editor_TimeLine_DiffMachine_TimeLineDiffMachine-MUSIC.cpp:

Program Listing for File TimeLineDiffMachine-MUSIC.cpp
======================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_DiffMachine_TimeLineDiffMachine-MUSIC.cpp>` (``include/core/editor/TimeLine/DiffMachine/TimeLineDiffMachine-MUSIC.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "TimeLineDiffMachine.hpp"
   
   #include "PDJE_LOG_SETTER.hpp"
   
   #include <cctype>
   #include <initializer_list>
   #include <optional>
   #include <string>
   #include <string_view>
   #include <vector>
   
   namespace PDJE_TIMELINE {
   namespace {
   
   using Core = TimeLineDiffMachineCore;
   
   std::string
   TrimCopyLocal(std::string_view sv)
   {
       std::size_t start = 0;
       std::size_t end   = sv.size();
       while (start < end &&
              std::isspace(static_cast<unsigned char>(sv[start])) != 0) {
           ++start;
       }
       while (end > start &&
              std::isspace(static_cast<unsigned char>(sv[end - 1])) != 0) {
           --end;
       }
       return std::string(sv.substr(start, end - start));
   }
   
   std::string
   TrimTrailingCommaLocal(std::string_view sv)
   {
       std::string out = TrimCopyLocal(sv);
       while (!out.empty() &&
              std::isspace(static_cast<unsigned char>(out.back())) != 0) {
           out.pop_back();
       }
       if (!out.empty() && out.back() == ',') {
           out.pop_back();
       }
       while (!out.empty() &&
              std::isspace(static_cast<unsigned char>(out.back())) != 0) {
           out.pop_back();
       }
       return out;
   }
   
   std::optional<nj>
   TryParseJsonObjectLocal(std::string_view sv)
   {
       try {
           auto parsed = nj::parse(sv.begin(), sv.end());
           if (!parsed.is_object()) {
               return std::nullopt;
           }
           return parsed;
       } catch (...) {
           return std::nullopt;
       }
   }
   
   bool
   JsonHasKeysLocal(const nj &obj, const std::initializer_list<const char *> &keys)
   {
       if (!obj.is_object()) {
           return false;
       }
       for (const auto *k : keys) {
           if (!obj.contains(k)) {
               return false;
           }
       }
       return true;
   }
   
   bool
   IsMusicMetaKeyLocal(const std::string &key)
   {
       return key == PDJE_JSON_TITLE || key == PDJE_JSON_COMPOSER ||
              key == PDJE_JSON_PATH || key == PDJE_JSON_FIRST_BEAT;
   }
   
   bool
   LooksLikeMusicBpmRowJson(const nj &obj)
   {
       return JsonHasKeysLocal(obj,
                               { PDJE_JSON_BPM, PDJE_JSON_BEAT, PDJE_JSON_SUBBEAT,
                                 PDJE_JSON_SEPARATE });
   }
   
   bool
   ValidateMusicObjectFragment(std::string_view fragment)
   {
       auto obj = TryParseJsonObjectLocal(TrimTrailingCommaLocal(fragment));
       return obj.has_value() && LooksLikeMusicBpmRowJson(*obj);
   }
   
   bool
   ValidateMusicFieldFragment(std::string_view fragment)
   {
       const auto norm = TrimTrailingCommaLocal(fragment);
       if (norm.empty()) {
           return false;
       }
   
       std::string wrapped = "{\n";
       wrapped += norm;
       wrapped += "\n}";
       auto parsed = TryParseJsonObjectLocal(wrapped);
       if (!parsed || parsed->empty()) {
           return false;
       }
       for (auto &[k, _] : parsed->items()) {
           if (!IsMusicMetaKeyLocal(k)) {
               return false;
           }
       }
       return true;
   }
   
   Core::RegionKindGuess
   MusicObjectRegionKind()
   {
       return Core::RegionKindGuess::MusicBpmRow;
   }
   
   Core::RegionKindGuess
   MusicFieldRegionKind()
   {
       return Core::RegionKindGuess::MusicMetaField;
   }
   
   std::vector<Core::RecoveryMode>
   MusicRecoveryModes()
   {
       return { Core::RecoveryMode::Object, Core::RecoveryMode::Field };
   }
   
   bool
   JsonToMusicArgsLocal(const nj &obj, MusicArgs &out)
   {
       try {
           out.bpm      = obj.at(PDJE_JSON_BPM).get<DONT_SANITIZE>();
           out.beat     = obj.at(PDJE_JSON_BEAT).get<uint64_t>();
           out.subBeat  = obj.at(PDJE_JSON_SUBBEAT).get<uint64_t>();
           out.separate = obj.at(PDJE_JSON_SEPARATE).get<uint64_t>();
           return true;
       } catch (const std::exception &e) {
           critlog("timeline diff music: MusicArgs parse failed.");
           critlog(e.what());
           return false;
       }
   }
   
   bool
   AppendMusicRegion(const Core::JsonizedRegion &region, TimeLineSemanticDiffResult &result)
   {
       const bool removed = region.side == Core::DiffSide::Origin;
   
       switch (region.region_kind) {
       case Core::RegionKindGuess::MusicBpmRow: {
           if (!region.json_value.is_object()) {
               critlog("timeline diff music: bpm region json is not object.");
               return false;
           }
           MusicArgs args;
           if (!JsonToMusicArgsLocal(region.json_value, args)) {
               return false;
           }
           if (removed) {
               result.musicBpmRemoved.push_back(args);
           } else {
               result.musicBpmAdded.push_back(args);
           }
           return true;
       }
       case Core::RegionKindGuess::MusicMetaField:
           if (!region.json_value.is_object() || region.json_value.empty()) {
               critlog("timeline diff music: metadata region json invalid.");
               return false;
           }
           for (auto &[k, v] : region.json_value.items()) {
               if (!IsMusicMetaKeyLocal(k)) {
                   critlog("timeline diff music: unsupported metadata key.");
                   critlog(k);
                   return false;
               }
               MusicMetaFieldDiff meta;
               meta.key   = k;
               meta.value = v.is_string() ? v.get<DONT_SANITIZE>() : v.dump();
               if (removed) {
                   result.musicMetaRemoved.push_back(std::move(meta));
               } else {
                   result.musicMetaAdded.push_back(std::move(meta));
               }
           }
           return true;
       default:
           critlog("timeline diff music: unexpected region kind.");
           return false;
       }
   }
   
   } // namespace
   
   template <>
   std::optional<TimeLineSemanticDiffResult>
   TimeLineDiffMachine<MUSIC_W>::Run()
   {
       TypeHooks hooks{};
       hooks.validate_object    = &ValidateMusicObjectFragment;
       hooks.validate_field     = &ValidateMusicFieldFragment;
       hooks.object_region_kind = &MusicObjectRegionKind;
       hooks.field_region_kind  = &MusicFieldRegionKind;
       hooks.recovery_modes     = &MusicRecoveryModes;
       hooks.append_region      = &AppendMusicRegion;
       return RunPipelineCore(TimeLineDiffKind::MUSIC, hooks);
   }
   
   } // namespace PDJE_TIMELINE
