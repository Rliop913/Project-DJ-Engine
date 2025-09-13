
.. _program_listing_file_include_core_editor_featureWrapper_GitWrap_Log_gitLog.cpp:

Program Listing for File gitLog.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_featureWrapper_GitWrap_Log_gitLog.cpp>` (``include/core/editor/featureWrapper/GitWrap/Log/gitLog.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "gitLog.hpp"
   
   #include "PDJE_LOG_SETTER.hpp"
   #include "editorBranch.hpp"
   #include <array>
   using namespace gitwrap;
   #define HASH_KNUTH 0x9e3779b9
   
   std::size_t
   logHandle::OID_HASHER::operator()(const git_oid &oid) const noexcept
   {
       uint64_t part0 =
           std::bit_cast<uint64_t>(std::array<unsigned char, 8>{ oid.id[0],
                                                                 oid.id[1],
                                                                 oid.id[2],
                                                                 oid.id[3],
                                                                 oid.id[4],
                                                                 oid.id[5],
                                                                 oid.id[6],
                                                                 oid.id[7] });
       uint64_t part1 =
           std::bit_cast<uint64_t>(std::array<unsigned char, 8>{ oid.id[8],
                                                                 oid.id[9],
                                                                 oid.id[10],
                                                                 oid.id[11],
                                                                 oid.id[12],
                                                                 oid.id[13],
                                                                 oid.id[14],
                                                                 oid.id[15] });
       uint32_t part2 = std::bit_cast<uint32_t>(std::array<unsigned char, 4>{
           oid.id[16], oid.id[17], oid.id[18], oid.id[19] });
   
       // 해시 결합
       std::size_t seed = 0;
   
       seed ^=
           std::hash<uint64_t>()(part0) + HASH_KNUTH + (seed << 6) + (seed >> 2);
       seed ^=
           std::hash<uint64_t>()(part1) + HASH_KNUTH + (seed << 6) + (seed >> 2);
       seed ^=
           std::hash<uint32_t>()(part2) + HASH_KNUTH + (seed << 6) + (seed >> 2);
   
       return seed;
   }
   
   bool
   logHandle::WalkBranch(const DONT_SANITIZE &branchName)
   {
   
       git_revwalk *walker = nullptr;
       git_revwalk_new(&walker, repoPointer);
   
       git_revwalk_sorting(walker, GIT_SORT_TIME);
   
       // 브랜치 참조 추가
       auto refBranchName =
           branch::ToBranchRefName<const DONT_SANITIZE &>(branchName);
   
       if (git_revwalk_push_ref(walker, refBranchName.c_str()) != 0) {
           critlog(
               "failed to revwalk push ref. from logHandle WalkBranch. gitLog: ");
           critlog(git_error_last()->message);
           git_revwalk_free(walker);
           return false;
       }
   
       git_oid    oid;
       git_oid    child_oid = { { 0 } };
       BranchHead bh;
       bh.BranchName = branchName;
   
       bool FLAG_ROOT_REACHED  = true;
       bool FLAG_DID_SOMETHING = false;
       while (!git_revwalk_next(&oid, walker)) {
           git_commit *commitref = nullptr;
           if (git_commit_lookup(&commitref, repoPointer, &oid) != 0) {
               continue;
           }
           if (logs.contains(oid)) {
               git_commit_free(commitref);
               FLAG_DID_SOMETHING = true;
               FLAG_ROOT_REACHED  = false;
               break;
           } else {
               FLAG_DID_SOMETHING    = true;
               auto          authref = git_commit_author(commitref);
               DONT_SANITIZE msg     = git_commit_message(commitref);
               if (git_oid_is_zero(&child_oid) == 1) {
                   git_oid_cpy(&bh.head, &oid);
               }
               AddLog(oid, child_oid, authref, msg);
               git_oid_cpy(&child_oid, &oid);
               git_commit_free(commitref);
           }
       }
       if (FLAG_DID_SOMETHING && FLAG_ROOT_REACHED) {
           git_oid_cpy(&ROOT_ID, &child_oid);
       }
   
       if (git_oid_is_zero(&bh.head) == 0) {
           heads.push_back(bh);
       }
       git_revwalk_free(walker);
       return true;
   }
   
   void
   logHandle::AddLog(const git_oid        &id,
                     git_oid              &ChildID,
                     const git_signature *&sign,
                     const DONT_SANITIZE  &msg)
   {
   
       log templog;
       git_oid_cpy(&templog.now.commitID, &id);
       templog.now.commitPointer = nullptr;
       templog.now.msg           = msg;
       templog.authName          = sign->name;
       templog.authEmail         = sign->email;
       if (git_oid_is_zero(&ChildID) == 0) {
           if (logs.contains(ChildID)) {
               git_oid_cpy(&logs[ChildID].parentID, &id);
           }
       }
   
       logs[id] = templog;
   }
