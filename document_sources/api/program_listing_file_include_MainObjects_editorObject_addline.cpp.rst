
.. _program_listing_file_include_MainObjects_editorObject_addline.cpp:

Program Listing for File addline.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_addline.cpp>` (``include/MainObjects/editorObject/addline.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template<> 
   PDJE_API
   bool
   editorObject::AddLine(const EDIT_ARG_MUSIC& obj)
   {
       auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(obj.musicName);
       if(!safeMus){
           critlog("Music name is not sanitized from editorObject Addline. musicName: ");
           critlog(obj.musicName);
           return false;
       }
       for(auto& i : E_obj->musicHandle){
           if(i.musicName == safeMus.value()){
               i.jsonh.add(obj.arg);
               return DefaultSaveFuntion<EDIT_ARG_MUSIC>(i, obj);
           }
       }
       warnlog("music is not exists. from editorObject AddLine(Music obj)");
       return false;
   }
   
   template<> 
   PDJE_API
   bool
   editorObject::AddLine(const EDIT_ARG_MIX& obj)
   {
       if(obj.type == TypeEnum::LOAD){
           auto safeObj = obj;
           auto first = PDJE_Name_Sanitizer::sanitizeFileName(safeObj.first);
           auto second = PDJE_Name_Sanitizer::sanitizeFileName(safeObj.second);
           if(!first || !second){
               critlog("Mix name is not sanitized from editorObject Addline. first: ");
               critlog(obj.first);
               critlog("second: ");
               critlog(obj.second);
               return false;
           }
           safeObj.first = first.value();
           safeObj.second = second.value();
           if(!E_obj->mixHandle.second.add(safeObj)){
               critlog("load Mix add failed from editorObject Addline. first: ");
               critlog(obj.first);
               critlog("second: ");
               critlog(obj.second);
               return false;
           }
       }
       else{
           if(!E_obj->mixHandle.second.add(obj)){
               critlog("Mix add failed from editorObject Addline. obj: ");
               critlog(obj.first);
               critlog("second: ");
               critlog(obj.second);
               
               return false;
           }
       }
       return DefaultSaveFuntion<EDIT_ARG_MIX>();
   }
   
   template<> 
   PDJE_API
   bool
   editorObject::AddLine(const EDIT_ARG_NOTE& obj)
   {
   
       if(!E_obj->noteHandle.second.add(obj)){
           critlog("Note add failed from editorObject Addline. obj: ");
           critlog(obj.first);
           critlog("second: ");
           critlog(obj.second);
           critlog("third: ");
           critlog(obj.third);
           return false;
       }
       return DefaultSaveFuntion<EDIT_ARG_NOTE>();
   }
   
   
   template<> 
   PDJE_API
   bool
   editorObject::AddLine(const EDIT_ARG_KEY_VALUE& obj)
   {
   
       if(!E_obj->KVHandler.second.add(obj)) {
           critlog("KV add failed from editorObject Addline. obj: ");
           critlog(obj.first);
           critlog("second: ");
           critlog(obj.second);
           
           return false;
       }
       return DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>();
   }
   
   bool 
   editorObject::AddLine(const UNSANITIZED& musicName, const DONT_SANITIZE& firstBar)
   {
       auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
       if(!safeMus){
           critlog("Music name is not sanitized from editorObject Addline. musicName: ");
           critlog(musicName);
           return false;
       }
       for(auto& i : E_obj->musicHandle){
           if(i.musicName == safeMus){
               i.jsonh["FIRST_BAR"] = firstBar;
               return true;
           }
       }
       warnlog("music is not exists. from editorObject AddLine(musicName, firstBar)");
       
       return false;
   }
