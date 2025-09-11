
.. _program_listing_file_core_include_MainObjects_editorObject_getdiff.cpp:

Program Listing for File getdiff.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_MainObjects_editorObject_getdiff.cpp>` (``core_include\MainObjects\editorObject\getdiff.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API DiffResult
   editorObject::GetDiff<EDIT_ARG_MIX>(const gitwrap::commit &oldTimeStamp,
                                       const gitwrap::commit &newTimeStamp)
   {
       return std::move(
           E_obj->mixHandle.first->GetDiff(oldTimeStamp, newTimeStamp));
   }
   
   template <>
   PDJE_API DiffResult
   editorObject::GetDiff<EDIT_ARG_NOTE>(const gitwrap::commit &oldTimeStamp,
                                        const gitwrap::commit &newTimeStamp)
   {
       return std::move(
           E_obj->noteHandle.first->GetDiff(oldTimeStamp, newTimeStamp));
   }
   
   template <>
   PDJE_API DiffResult
   editorObject::GetDiff<EDIT_ARG_KEY_VALUE>(const gitwrap::commit &oldTimeStamp,
                                             const gitwrap::commit &newTimeStamp)
   {
       return std::move(
           E_obj->KVHandler.first->GetDiff(oldTimeStamp, newTimeStamp));
   }
   
   template <>
   PDJE_API DiffResult
   editorObject::GetDiff<EDIT_ARG_MUSIC>(const gitwrap::commit &oldTimeStamp,
                                         const gitwrap::commit &newTimeStamp)
   {
       for (auto &i : E_obj->musicHandle) {
           auto restemp = i.gith->GetDiff(oldTimeStamp, newTimeStamp);
           if (!restemp.NewLines.empty() || !restemp.OldLines.empty()) {
               return std::move(restemp);
           }
       }
       warnlog("cannot find music. from editorObject GetDiff(Music obj)");
       return DiffResult();
   }
