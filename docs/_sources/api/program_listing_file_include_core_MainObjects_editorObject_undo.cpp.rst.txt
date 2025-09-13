
.. _program_listing_file_include_core_MainObjects_editorObject_undo.cpp:

Program Listing for File undo.cpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_editorObject_undo.cpp>` (``include/core/MainObjects/editorObject/undo.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API bool
   editorObject::Undo<EDIT_ARG_NOTE>()
   {
       return E_obj->noteHandle.first->Undo();
   }
   
   template <>
   PDJE_API bool
   editorObject::Undo<EDIT_ARG_MIX>()
   {
       return E_obj->mixHandle.first->Undo();
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
   
       for (auto &i : E_obj->musicHandle) {
           if (i.musicName == safeMus) {
               return i.gith->Undo();
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
       return E_obj->KVHandler.first->Undo();
   }
