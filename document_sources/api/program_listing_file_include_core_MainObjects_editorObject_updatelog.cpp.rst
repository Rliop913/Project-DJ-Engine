
.. _program_listing_file_include_core_MainObjects_editorObject_updatelog.cpp:

Program Listing for File updatelog.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_editorObject_updatelog.cpp>` (``include\core\MainObjects\editorObject\updatelog.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API void
   editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>()
   {
       edit_core->KVHandle->UpdateLogs();
   }
   
   template <>
   PDJE_API void
   editorObject::UpdateLog<EDIT_ARG_MIX>()
   {
       edit_core->mixHandle->UpdateLogs();
   }
   
   template <>
   PDJE_API void
   editorObject::UpdateLog<EDIT_ARG_NOTE>()
   {
       edit_core->noteHandle->UpdateLogs();
   }
   
   template <>
   PDJE_API void
   editorObject::UpdateLog<EDIT_ARG_MUSIC>()
   {
       for (auto &i : edit_core->musicHandle) {
           i.handle->UpdateLogs();
       }
   }
