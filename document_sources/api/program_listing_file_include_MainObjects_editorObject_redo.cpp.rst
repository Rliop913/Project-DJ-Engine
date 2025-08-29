
.. _program_listing_file_include_MainObjects_editorObject_redo.cpp:

Program Listing for File redo.cpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_redo.cpp>` (``include\MainObjects\editorObject\redo.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API bool
   editorObject::Redo<EDIT_ARG_MIX>()
   {
       return E_obj->mixHandle.first->Redo();
   }
   
   template <>
   PDJE_API bool
   editorObject::Redo<EDIT_ARG_NOTE>()
   {
       return E_obj->noteHandle.first->Redo();
   }
   template <>
   PDJE_API bool
   editorObject::Redo<EDIT_ARG_MUSIC>(const UNSANITIZED &musicName)
   {
       auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
       if (!safeMus) {
           critlog(
               "Music name is not sanitized from editorObject Redo. musicName: ");
           critlog(musicName);
           return false;
       }
       for (auto &i : E_obj->musicHandle) {
           if (i.musicName == safeMus) {
               return i.gith->Redo();
           }
       }
       warnlog(
           "music is not exists. from editorObject Redo(Music obj). musicName:");
       warnlog(musicName);
       return false;
   }
   template <>
   PDJE_API bool
   editorObject::Redo<EDIT_ARG_KEY_VALUE>()
   {
       return E_obj->KVHandler.first->Redo();
   }
