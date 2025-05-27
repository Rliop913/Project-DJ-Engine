
.. _program_listing_file_include_MainObjects_editorObject_redo.cpp:

Program Listing for File redo.cpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_redo.cpp>` (``include/MainObjects/editorObject/redo.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template<>
   bool
   editorObject::Redo<EDIT_ARG_MIX>()
   {
       return E_obj->mixHandle.first.Redo(); 
   }
   
   template<>
   bool
   editorObject::Redo<EDIT_ARG_NOTE>()
   {
       return E_obj->noteHandle.first.Redo();
   }
   template<>
   bool
   editorObject::Redo<EDIT_ARG_MUSIC>(const std::string& musicName)
   {
       for(auto& i : E_obj->musicHandle){
           if(i.musicName == musicName){
               return i.gith.Redo();
           }
       }
       return false;
   }
   template<>
   bool
   editorObject::Redo<EDIT_ARG_KEY_VALUE>()
   {
       return E_obj->KVHandler.first.Redo();
   }
