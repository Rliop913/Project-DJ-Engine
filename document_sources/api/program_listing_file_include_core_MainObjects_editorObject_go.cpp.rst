
.. _program_listing_file_include_core_MainObjects_editorObject_go.cpp:

Program Listing for File go.cpp
===============================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_editorObject_go.cpp>` (``include\core\MainObjects\editorObject\go.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_MIX>(const DONT_SANITIZE &commitOID)
   {
       return edit_core->mixHandle->Go(commitOID);
   }
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_NOTE>(const DONT_SANITIZE &commitOID)
   {
       return edit_core->noteHandle->Go(commitOID);
   }
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE &commitOID)
   {
       return edit_core->KVHandle->Go(commitOID);
   }
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_MUSIC>(const DONT_SANITIZE &commitOID)
   {
       for (auto &i : edit_core->musicHandle) {
           if (i.handle->Go(commitOID)) {
               return true;
           }
       }
       warnlog("cannot find music. from editorObject Go(Music obj)");
       return false;
   }
