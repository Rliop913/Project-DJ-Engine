
.. _program_listing_file_include_core_MainObjects_editorObject_getall.cpp:

Program Listing for File getall.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_editorObject_getall.cpp>` (``include/core/MainObjects/editorObject/getall.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API void
   editorObject::getAll(
       std::function<void(const EDIT_ARG_KEY_VALUE &obj)> jsonCallback)
   {
       E_obj->KVHandler.second.getAll(jsonCallback);
   }
   
   template <>
   PDJE_API void
   editorObject::getAll(std::function<void(const EDIT_ARG_MIX &obj)> jsonCallback)
   {
       E_obj->mixHandle.second.getAll(jsonCallback);
   }
   
   template <>
   PDJE_API void
   editorObject::getAll(std::function<void(const EDIT_ARG_NOTE &obj)> jsonCallback)
   {
       E_obj->noteHandle.second.getAll(jsonCallback);
   }
   
   template <>
   PDJE_API void
   editorObject::getAll(
       std::function<void(const EDIT_ARG_MUSIC &obj)> jsonCallback)
   {
       for (auto &i : E_obj->musicHandle) {
           i.jsonh.getAll(jsonCallback);
       }
   }
