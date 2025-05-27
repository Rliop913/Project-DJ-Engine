
.. _program_listing_file_include_MainObjects_editorObject_go.cpp:

Program Listing for File go.cpp
===============================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_go.cpp>` (``include/MainObjects/editorObject/go.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template<>
   bool
   editorObject::Go<EDIT_ARG_MIX>(const std::string& branchName, git_oid* commitID)
   {
       return E_obj->mixHandle.first.Go(branchName, commitID);
   }
   
   template<>
   bool
   editorObject::Go<EDIT_ARG_NOTE>(const std::string& branchName, git_oid* commitID)
   {
       return E_obj->noteHandle.first.Go(branchName, commitID);
   }
   
   template<>
   bool
   editorObject::Go<EDIT_ARG_KEY_VALUE>(const std::string& branchName, git_oid* commitID)
   {
       return E_obj->KVHandler.first.Go(branchName, commitID);
   }
   
   template<>
   bool
   editorObject::Go<EDIT_ARG_MUSIC>(const std::string& branchName, git_oid* commitID)
   {
       for(auto& i : E_obj->musicHandle){
           if(i.gith.Go(branchName, commitID)) return true;
       }
       return false;
   }
   
