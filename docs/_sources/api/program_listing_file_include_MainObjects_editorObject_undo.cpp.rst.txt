
.. _program_listing_file_include_MainObjects_editorObject_undo.cpp:

Program Listing for File undo.cpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_undo.cpp>` (``include/MainObjects/editorObject/undo.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template<>
   bool
   editorObject::Undo<EDIT_ARG_NOTE>()
   {
       return E_obj->noteHandle.first.Undo();
   }
   
   template<>
   bool
   editorObject::Undo<EDIT_ARG_MIX>()
   {
       return E_obj->mixHandle.first.Undo();
   }
   
   template<>
   bool
   editorObject::Undo<EDIT_ARG_MUSIC>(const std::string& musicName)
   {
       for(auto& i : E_obj->musicHandle){
           if(i.musicName == musicName){
               return i.gith.Undo();
           }
       }
       return false;
   }
   
   template<>
   bool
   editorObject::Undo<EDIT_ARG_KEY_VALUE>()
   {
       return E_obj->KVHandler.first.Undo();
   }
