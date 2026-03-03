
.. _program_listing_file_include_core_editor_TimeLine_TimeLineDiffTypes.hpp:

Program Listing for File TimeLineDiffTypes.hpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_TimeLineDiffTypes.hpp>` (``include/core/editor/TimeLine/TimeLineDiffTypes.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "GitDatas.hpp"
   #include "jsonWrapper.hpp"
   
   #include <optional>
   #include <vector>
   
   namespace PDJE_TIMELINE {
   
   enum class TimeLineDiffKind { KV, MIX, NOTE, MUSIC };
   
   struct MusicMetaFieldDiff {
       KEY           key;
       DONT_SANITIZE value;
   };
   
   struct TimeLineSemanticDiffResult {
       TimeLineDiffKind kind = TimeLineDiffKind::KV;
       OID              origin;
       OID              compare;
   
       std::vector<KEY_VALUE> kvRemoved;
       std::vector<KEY_VALUE> kvAdded;
   
       std::vector<MixArgs> mixRemoved;
       std::vector<MixArgs> mixAdded;
   
       std::vector<NoteArgs> noteRemoved;
       std::vector<NoteArgs> noteAdded;
   
       std::vector<MusicArgs> musicBpmRemoved;
       std::vector<MusicArgs> musicBpmAdded;
   
       std::vector<MusicMetaFieldDiff> musicMetaRemoved;
       std::vector<MusicMetaFieldDiff> musicMetaAdded;
   
       bool
       Empty() const
       {
           return kvRemoved.empty() && kvAdded.empty() && mixRemoved.empty() &&
                  mixAdded.empty() && noteRemoved.empty() && noteAdded.empty() &&
                  musicBpmRemoved.empty() && musicBpmAdded.empty() &&
                  musicMetaRemoved.empty() && musicMetaAdded.empty();
       }
   };
   
   std::optional<TimeLineSemanticDiffResult>
   BuildTimeLineSemanticDiff(git_repository          *repo,
                             const std::string       &target_file,
                             const OID               &origin,
                             const OID               &compare,
                             TimeLineDiffKind         kind);
   
   }; // namespace PDJE_TIMELINE
