
.. _program_listing_file_include_MainObjects_editorObject_getlogwithjsongraph.cpp:

Program Listing for File getlogwithjsongraph.cpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_getlogwithjsongraph.cpp>` (``include\MainObjects\editorObject\getlogwithjsongraph.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API DONT_SANITIZE
   editorObject::GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>()
   {
       return E_obj->KVHandler.first->GetLogWithJSONGraph();
   }
   
   template <>
   PDJE_API DONT_SANITIZE
   editorObject::GetLogWithJSONGraph<EDIT_ARG_MIX>()
   {
       return E_obj->mixHandle.first->GetLogWithJSONGraph();
   }
   
   template <>
   PDJE_API DONT_SANITIZE
   editorObject::GetLogWithJSONGraph<EDIT_ARG_NOTE>()
   {
       return E_obj->noteHandle.first->GetLogWithJSONGraph();
   }
   
   template <>
   PDJE_API DONT_SANITIZE
   editorObject::GetLogWithJSONGraph<EDIT_ARG_MUSIC>(const UNSANITIZED &musicName)
   {
       auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
       if (!safeMus) {
           critlog("Music name is not sanitized from editorObject "
                   "GetLogWithJSONGraph. musicName: ");
           critlog(musicName);
           return DONT_SANITIZE();
       }
       for (auto &i : E_obj->musicHandle) {
           if (i.musicName == safeMus) {
               return i.gith->GetLogWithJSONGraph();
           }
       }
       warnlog("music is not exists. from editorObject GetLogWithJSONGraph(Music "
               "obj). musicName:");
       warnlog(musicName);
       return DONT_SANITIZE();
   }
