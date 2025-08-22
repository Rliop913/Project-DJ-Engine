
.. _program_listing_file_include_editor_featureWrapper_GitWrap_gitWrapper.cpp:

Program Listing for File gitWrapper.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_gitWrapper.cpp>` (``include\editor\featureWrapper\GitWrap\gitWrapper.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "gitWrapper.hpp"
   #include "editorCommit.hpp"
   #include "git2/commit.h"
   #include "git2/oid.h"
   #include <string>
   #include <chrono>
   #include <sstream>
   #include "PDJE_LOG_SETTER.hpp"
   // #include "CommitFinder.hpp"
   
   MAYBE_BLAME
   GitWrapper::Blame(const fs::path& filepath, const gitwrap::commit& newCommit, const gitwrap::commit& oldCommit)
   {
       auto newBlame = BlameController();
       git_blame_options opts;
       git_blame_options_init(&opts, GIT_BLAME_OPTIONS_VERSION);
       opts.newest_commit = newCommit.commitID;
       opts.oldest_commit = oldCommit.commitID;
       if(newBlame.BlameOpen(repo, filepath, &opts)){
           return std::move(newBlame);
       }
       else{
           critlog("failed to blame. from GitWrapper Blame. gitLog: ");
           critlog(git_error_last()->message);
           return std::nullopt;
       }
   }
   
   DiffResult
   GitWrapper::diff(const gitwrap::commit& oldCommit, const gitwrap::commit& newCommit)
   {
       auto DiffHandle = DiffController();
       DiffResult results;
       if(oldCommit.commitPointer == nullptr) {
           critlog("old commit pointer is null. from GitWrapper diff.");
           return results;
       }
   
       if(newCommit.commitPointer != nullptr){
           if(!DiffHandle.CommitToCommit(repo, newCommit.commitID, oldCommit.commitID)) {
               critlog("failed to diff commit to commit. from GitWrapper diff. gitLog: ");
               critlog(git_error_last()->message);
               return results;
           }
       }
       else{
           if(!DiffHandle.CommitToNow(repo, oldCommit.commitID)) {
               critlog("failed to diff commit to now. from GitWrapper diff. gitLog: ");
               critlog(git_error_last()->message);
               return results;
           }
       }
       DiffHandle.execute(&results);
       return results;
   }
   
   bool
   GitWrapper::add(const fs::path& path)
   {
       if(addIndex.has_value()){
           addIndex.reset();
       }
       addIndex.emplace();
       if(!addIndex->open(repo)){
           critlog("failed to open repo. from GitWrapper add. gitLog: ");
           critlog(git_error_last()->message);
           return false;
       }
       if(!addIndex->addFile(path)) {
           critlog("failed to add file. from GitWrapper add. gitLog: ");
           critlog(git_error_last()->message);
           return false;
       }
       return true;
   }
   
   
   
   
   
   bool
   GitWrapper::open(const fs::path& path)
   {
       
       auto safeStr = path.generic_string();
       if(git_repository_open(&repo, safeStr.c_str()) == 0){
           handleBranch.emplace(repo);
           return true;
       }
       else{
           auto res = git_repository_init(&repo, safeStr.c_str(), false);
           if(res == 0){
               handleBranch.emplace(repo);
               return true;
           }
           else{
               critlog("failed to open & init repository. from GitWrapper open. gitLog: ");
               critlog(git_error_last()->message);
               return false;
           }
       }
   }
   
   
   bool
   GitWrapper::close()
   {
       if(repo == nullptr){
           warnlog("failed to close. repo is nullptr. from GitWrapper close. gitLog: ");
           warnlog(git_error_last()->message);
           return false;
       }
       git_repository_free(repo);
       repo=nullptr;
       return true;
   }
   
   GitWrapper::GitWrapper()
   {
       git_libgit2_init();
   }
   
   GitWrapper::~GitWrapper()
   {
       if(repo != nullptr){
           git_repository_free(repo);
       }
       if(addIndex.has_value()) {
           addIndex.reset();
       }
       git_libgit2_shutdown();
   }
   
   bool
   GitWrapper::commit(git_signature* sign, const DONT_SANITIZE& message)
   {
       git_oid tree_id, commit_id, parent_id;
       git_tree* tree = nullptr;
       git_commit* parent_commit = nullptr;
       bool result = false;
   
       if(!handleBranch.has_value()){
           critlog("handleBranch has no value. from GitWrapper commit. gitLog: ");
           critlog(git_error_last()->message);
           return false;
       }
       if(handleBranch->FLAG_TEMP_CHECKOUT.has_value()){
           auto tempcommit = gitwrap::commit();
   
           git_commit_lookup(
               &(tempcommit.commitPointer),
               repo,
               &(handleBranch->FLAG_TEMP_CHECKOUT.value()));
   
           handleBranch->MakeNewFromCommit(
               tempcommit, GenTimeStamp()
               );
       }
   
       if (!addIndex.has_value()) {
           critlog("failed because addIndex has no value. from GitWrapper commit. gitLog: ");
           critlog(git_error_last()->message);
           goto cleanup;
       }
       if (git_index_write_tree(&tree_id, addIndex->index) != 0) {
           critlog("failed because index write tree failed. from GitWrapper commit. gitLog: ");
           critlog(git_error_last()->message);
           goto cleanup;
       }
       if (git_tree_lookup(&tree, repo, &tree_id) != 0) {
           critlog("failed because lookup tree failed. from GitWrapper commit. gitLog: ");
           critlog(git_error_last()->message);
           goto cleanup;
       }
   
       // 부모 커밋이 있는 경우
       if (git_reference_name_to_id(&parent_id, repo, "HEAD") == 0 &&
           git_commit_lookup(&parent_commit, repo, &parent_id) == 0) {
           // const git_commit* parents[1] = { parent_commit };
           if (git_commit_create_v(
                   &commit_id, repo, "HEAD", sign, sign, nullptr,
                   message.c_str(), tree, 1, parent_commit
               ) == 0) {
               result = true;
           }
       } else {
           // 최초 커밋(부모 없음)
           if (git_commit_create_v(
                   &commit_id, repo, "HEAD", sign, sign, nullptr,
                   message.c_str(), tree, 0
               ) == 0) {
               result = true;
           }
       }
   
   cleanup:
       if (tree) git_tree_free(tree);
       if (parent_commit) git_commit_free(parent_commit);
       addIndex.reset();
       if(!result){
           critlog("something failed. from GitWrapper commit. gitLog: ");
           critlog(git_error_last()->message);
       }
       return result;
   }
   
   
   bool
   GitWrapper::log()
   {
       if(!log_hdl.has_value()){
           log_hdl.emplace(repo);
       }
       if(!handleBranch.has_value()){
           critlog("handleBranch has no value. from GitWrapper log. gitLog: ");
           critlog(git_error_last()->message);
           return false;
       }
       auto branches = handleBranch->ShowExistBranch();
       for(auto& i : branches){
           if(!log_hdl->WalkBranch(i)){
               critlog("walkBranch failed. from GitWrapper log. gitLog: ");
               critlog(git_error_last()->message);
              return false;
           }
       }
       return true;
   
   }
   
   bool
   GitWrapper::log(const DONT_SANITIZE& branchName)
   {
       if(!log_hdl.has_value()){
           log_hdl.emplace(repo);
       }
       if(!log_hdl->WalkBranch(branchName)){
           critlog("walkBranch failed. from GitWrapper log(branchName). gitLog: ");
           critlog(git_error_last()->message);
           return false;
       }
       return true;
   }
   
   DONT_SANITIZE
   GitWrapper::GenTimeStamp()
   {
       using namespace std::chrono;
   
       auto now = system_clock::now();
   
       std::time_t tt = system_clock::to_time_t(now);
       std::tm tm{};
       #if defined(_WIN32)
           localtime_s(&tm, &tt);
       #else
           localtime_r(&tt, &tm);
       #endif
   
           auto ms = duration_cast<milliseconds>(now - system_clock::from_time_t(tt)).count();
   
           std::ostringstream oss;
           oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
           oss << '.' << std::setw(3) << std::setfill('0') << ms;
   
           return oss.str();
   }
