
.. _program_listing_file_include_MainObjects_editorObject_go.cpp:

Program Listing for File go.cpp
===============================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_go.cpp>` (``include\MainObjects\editorObject\go.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_MIX>(const DONT_SANITIZE &branchName,
                                  const DONT_SANITIZE &commitOID)
   {
       git_oid targetOID{};
       if (git_oid_fromstr(&targetOID, commitOID.c_str()) != GIT_OK) {
           critlog("oid string to git oid convert failed on "
                   "editorObject::Go<EDIT_ARG_MIX>. printing git err msg: ");
           critlog(git_error_last()->message);
   
           return false;
       }
       return E_obj->mixHandle.first->Go(branchName, &targetOID);
   }
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_NOTE>(const DONT_SANITIZE &branchName,
                                   const DONT_SANITIZE &commitOID)
   {
       git_oid targetOID{};
       if (git_oid_fromstr(&targetOID, commitOID.c_str()) != GIT_OK) {
           critlog("oid string to git oid convert failed on "
                   "editorObject::Go<EDIT_ARG_NOTE>. printing git err msg: ");
           critlog(git_error_last()->message);
           return false;
       }
       return E_obj->noteHandle.first->Go(branchName, &targetOID);
   }
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE &branchName,
                                        const DONT_SANITIZE &commitOID)
   {
       git_oid targetOID{};
       if (git_oid_fromstr(&targetOID, commitOID.c_str()) != GIT_OK) {
           critlog("oid string to git oid convert failed on "
                   "editorObject::Go<EDIT_ARG_KEY_VALUE>. printing git err msg: ");
           critlog(git_error_last()->message);
           return false;
       }
       return E_obj->KVHandler.first->Go(branchName, &targetOID);
   }
   
   template <>
   PDJE_API bool
   editorObject::Go<EDIT_ARG_MUSIC>(const DONT_SANITIZE &branchName,
                                    const DONT_SANITIZE &commitOID)
   {
       git_oid targetOID{};
       if (git_oid_fromstr(&targetOID, commitOID.c_str()) != GIT_OK) {
           critlog("oid string to git oid convert failed on "
                   "editorObject::Go<EDIT_ARG_MUSIC>. printing git err msg: ");
           critlog(git_error_last()->message);
           return false;
       }
       for (auto &i : E_obj->musicHandle) {
           if (i.gith->Go(branchName, &targetOID))
               return true;
       }
       warnlog("cannot find music. from editorObject Go(Music obj)");
       return false;
   }
