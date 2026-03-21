
.. _program_listing_file_include_core_MainObjects_editorObject_getdiff.cpp:

Program Listing for File getdiff.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_editorObject_getdiff.cpp>` (``include/core/MainObjects/editorObject/getdiff.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MusicJsonHelper.hpp"
   #include "editorObject.hpp"
   
   namespace {
   
   template <typename TimeLineType>
   std::optional<PDJE_TIMELINE::TimeLineSemanticDiffResult>
   DiffFromHandle(TimeLineType      *handle,
                  const char        *label,
                  const std::string &oldCommitOID,
                  const std::string &newCommitOID)
   {
       if (handle == nullptr) {
           critlog("editorObject diff target handle is nullptr. label: ");
           critlog(label);
           return std::nullopt;
       }
   
       return handle->Diff(oldCommitOID, newCommitOID);
   }
   
   std::vector<PDJE_TIMELINE::TimeLine<MUSIC_W> *>
   FindMatchingMusicHandles(PDJE_Editor *edit_core, const SANITIZED &musicTitle)
   {
       std::vector<PDJE_TIMELINE::TimeLine<MUSIC_W> *> matches;
       if (edit_core == nullptr) {
           return matches;
       }
   
       for (auto &i : edit_core->musicHandle) {
           if (i.handle != nullptr &&
               GetTitle(*i.handle->GetJson()) == musicTitle) {
               matches.push_back(i.handle.get());
           }
       }
   
       return matches;
   }
   
   } // namespace
   
   template <>
   std::optional<PDJE_TIMELINE::TimeLineSemanticDiffResult>
            editorObject::GetDiff<EDIT_ARG_MIX>(const DONT_SANITIZE &oldCommitOID,
                                       const DONT_SANITIZE &newCommitOID)
   {
       return DiffFromHandle(edit_core ? edit_core->mixHandle.get() : nullptr,
                             "mix",
                             oldCommitOID,
                             newCommitOID);
   }
   
   template <>
   std::optional<PDJE_TIMELINE::TimeLineSemanticDiffResult>
            editorObject::GetDiff<EDIT_ARG_NOTE>(const DONT_SANITIZE &oldCommitOID,
                                        const DONT_SANITIZE &newCommitOID)
   {
       return DiffFromHandle(edit_core ? edit_core->noteHandle.get() : nullptr,
                             "note",
                             oldCommitOID,
                             newCommitOID);
   }
   
   template <>
   std::optional<PDJE_TIMELINE::TimeLineSemanticDiffResult>
   editorObject::GetDiff<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE &oldCommitOID,
                                             const DONT_SANITIZE &newCommitOID)
   {
       return DiffFromHandle(edit_core ? edit_core->KVHandle.get() : nullptr,
                             "key-value",
                             oldCommitOID,
                             newCommitOID);
   }
   
   template <>
   std::optional<PDJE_TIMELINE::TimeLineSemanticDiffResult>
            editorObject::GetDiff<EDIT_ARG_MUSIC>(const UNSANITIZED   &musicName,
                                         const DONT_SANITIZE &oldCommitOID,
                                         const DONT_SANITIZE &newCommitOID)
   {
       auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
       if (!safeMus) {
           critlog("Music name is not sanitized from editorObject GetDiff. "
                   "musicName: ");
           critlog(musicName);
           return std::nullopt;
       }
   
       if (!edit_core) {
           critlog("editorObject diff failed because edit_core is nullptr.");
           return std::nullopt;
       }
   
       auto matches = FindMatchingMusicHandles(edit_core.get(), safeMus.value());
       if (matches.empty()) {
           warnlog("music is not exists. from editorObject GetDiff(Music obj). "
                   "musicName:");
           warnlog(musicName);
           return std::nullopt;
       }
   
       if (matches.size() == 1) {
           return matches.front()->Diff(oldCommitOID, newCommitOID);
       }
   
       std::optional<PDJE_TIMELINE::TimeLineSemanticDiffResult> matchedDiff;
       for (auto *handle : matches) {
           auto candidate = handle->Diff(oldCommitOID, newCommitOID);
           if (candidate.has_value()) {
               if (matchedDiff.has_value()) {
                   warnlog("music diff is ambiguous for duplicated titles. "
                           "Provide a more specific target.");
                   warnlog(musicName);
                   return std::nullopt;
               }
               matchedDiff = std::move(candidate);
           }
       }
   
       if (!matchedDiff.has_value()) {
           warnlog("cannot match music diff commits for requested music title. "
                   "musicName:");
           warnlog(musicName);
       }
   
       return matchedDiff;
   }
