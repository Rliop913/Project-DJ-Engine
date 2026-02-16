
.. _program_listing_file_include_core_MainObjects_editorObject_deleteline.cpp:

Program Listing for File deleteline.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_editorObject_deleteline.cpp>` (``include/core/MainObjects/editorObject/deleteline.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MusicJsonHelper.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "editorObject.hpp"
   
   int
   editorObject::deleteLine(const EDIT_ARG_MIX &obj,
                            bool                skipType,
                            bool                skipDetail)
   {
       const int deleted_lines =
           edit_core->mixHandle->DeleteData(obj, skipType, skipDetail);
       if (deleted_lines < 0) {
           critlog("failed to save deleted result.");
       }
       return deleted_lines;
   }
   
   template <>
   PDJE_API int
   editorObject::deleteLine(const EDIT_ARG_NOTE &obj)
   {
       const int deleted_lines = edit_core->noteHandle->DeleteData(obj);
       if (deleted_lines < 0) {
           critlog("failed to save deleted result.");
       }
       return deleted_lines;
   }
   
   template <>
   PDJE_API int
   editorObject::deleteLine(const EDIT_ARG_KEY_VALUE &obj)
   {
       const int deleted_lines = edit_core->KVHandle->DeleteData(obj.first);
       if (deleted_lines < 0) {
           critlog("failed to save deleted result.");
       }
       return deleted_lines;
   }
   
   template <>
   PDJE_API int
   editorObject::deleteLine(const EDIT_ARG_MUSIC &obj)
   {
       for (auto &i : edit_core->musicHandle) {
           if (GetTitle(*i.handle->GetJson()) == obj.musicName) {
               const int deleted_lines = i.handle->DeleteData(obj.arg);
               if (deleted_lines < 0) {
                   critlog("failed to save deleted result.");
               }
               return deleted_lines;
           }
       }
       warnlog("music is not exists. from editorObject deleteLine(Music obj)");
       return 0;
   }
