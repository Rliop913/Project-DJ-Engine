
.. _program_listing_file_include_core_MainObjects_editorObject_undo.cpp:

Program Listing for File undo.cpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_editorObject_undo.cpp>` (``include/core/MainObjects/editorObject/undo.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MusicJsonHelper.hpp"
   #include "editorObject.hpp"
   
   template <>
   PDJE_API bool
   editorObject::Undo<EDIT_ARG_NOTE>()
   {
       return edit_core->noteHandle->Undo();
   }
   
   template <>
   PDJE_API bool
   editorObject::Undo<EDIT_ARG_MIX>()
   {
       return edit_core->mixHandle->Undo();
   }
   
   template <>
   PDJE_API bool
   editorObject::Undo<EDIT_ARG_MUSIC>(const UNSANITIZED &musicName)
   {
       auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
       if (!safeMus) {
           critlog(
               "Music name is not sanitized from editorObject Undo. musicName: ");
           critlog(musicName);
           return false;
       }
   
       for (auto &i : edit_core->musicHandle) {
   
           if (GetTitle(*i.handle->GetJson()) == safeMus) {
               return i.handle->Undo();
           }
       }
       warnlog(
           "music is not exists. from editorObject Undo(Music obj). musicName:");
       warnlog(musicName);
   
       return false;
   }
   
   template <>
   PDJE_API bool
   editorObject::Undo<EDIT_ARG_KEY_VALUE>()
   {
       return edit_core->KVHandle->Undo();
   }
