
.. _program_listing_file_include_MainObjects_editorObject_getlogwithjsongraph.cpp:

Program Listing for File getlogwithjsongraph.cpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_getlogwithjsongraph.cpp>` (``include/MainObjects/editorObject/getlogwithjsongraph.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template<>
   std::string
   editorObject::GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>()
   {
       return E_obj->KVHandler.first.GetLogWithJSONGraph();
   }
   
   template<>
   std::string
   editorObject::GetLogWithJSONGraph<EDIT_ARG_MIX>()
   {
       return E_obj->mixHandle.first.GetLogWithJSONGraph();
   }
   
   template<>
   std::string
   editorObject::GetLogWithJSONGraph<EDIT_ARG_NOTE>()
   {
       return E_obj->noteHandle.first.GetLogWithJSONGraph();
   }
   
   template<>
   std::string
   editorObject::GetLogWithJSONGraph<EDIT_ARG_MUSIC>(const std::string& musicName)
   {
       for(auto& i : E_obj->musicHandle){
           if(i.musicName == musicName){
               return i.gith.GetLogWithJSONGraph();
           }
       }
       return std::string();
   }
   
   
   
   
