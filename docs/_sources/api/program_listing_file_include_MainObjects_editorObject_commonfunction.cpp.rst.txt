
.. _program_listing_file_include_MainObjects_editorObject_commonfunction.cpp:

Program Listing for File commonfunction.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_commonfunction.cpp>` (``include/MainObjects/editorObject/commonfunction.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   
   
   template<>
   bool
   editorObject::DefaultSaveFuntion<EDIT_ARG_MIX>()
   {
       if(!E_obj->mixHandle.second.save(mixFilePath.string())) return false;
       if(!E_obj->mixHandle.first.Save(mixFilePath.string(), GitWrapper::GenTimeStamp())) return false;
       return true;
   }
   
   template<>
   bool
   editorObject::DefaultSaveFuntion<EDIT_ARG_NOTE>()
   {
       if(!E_obj->noteHandle.second.save(noteFilePath.string())) return false;
       if(!E_obj->noteHandle.first.Save(noteFilePath.string(), GitWrapper::GenTimeStamp())) return false;
       return true;
   }
   
   template<>
   bool
   editorObject::DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>()
   {
       if(!E_obj->KVHandler.second.save(kvFilePath.string())) return false;
       if(!E_obj->KVHandler.first.Save(kvFilePath.string(), GitWrapper::GenTimeStamp())) return false;
       return true;
   }
   
   template<>
   bool
   editorObject::DefaultSaveFuntion<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj)
   {
       if(!i.jsonh.save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").string())) return false;
       if(!i.gith.Save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").string(), GitWrapper::GenTimeStamp())) return false;
       return true;
   }
