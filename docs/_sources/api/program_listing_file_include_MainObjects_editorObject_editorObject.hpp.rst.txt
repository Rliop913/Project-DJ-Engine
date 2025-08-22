
.. _program_listing_file_include_MainObjects_editorObject_editorObject.hpp:

Program Listing for File editorObject.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_editorObject.hpp>` (``include\MainObjects\editorObject\editorObject.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #include <optional>
   #include <filesystem>
   #include "editor.hpp"
   #include "audioPlayer.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "tempDB.hpp"
   
   struct PDJE_API EDIT_ARG_MUSIC{
       UNSANITIZED musicName; 
       
       MusicArgs arg; 
   };
   
   using EDIT_ARG_NOTE = NoteArgs;
   using EDIT_ARG_MIX  = MixArgs;
   using EDIT_ARG_KEY_VALUE = KEY_VALUE;
   using TITLE_COMPOSER = std::unordered_map<SANITIZED, SANITIZED>;
   
   class PDJE_API editorObject {
   private:
       std::optional<tempDB> projectLocalDB;
       fs::path projectRoot;
       fs::path mixFilePath;
       fs::path noteFilePath;
       fs::path kvFilePath;
       fs::path musicFileRootPath;
       std::optional<PDJE_Editor> E_obj;
   
       template<typename EDIT_ARG_TYPE>
       bool DefaultSaveFunction();
   
       template<typename EDIT_ARG_TYPE>
       bool DefaultSaveFunction(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj);
   
       trackdata makeTrackData(
           const UNSANITIZED& trackTitle, 
           TITLE_COMPOSER& titles);
   
   public:
       git_repository* getMixRepo(){
           if(E_obj.has_value()){
               return E_obj->mixHandle.first->gw.repo;
           }
           else return nullptr;
       }
   
       git_repository* getMusicRepo(const UNSANITIZED& Title){
           auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(Title);
           if(!safeTitle){
               return nullptr;
           }
           if(E_obj.has_value()){
               for(auto& music : E_obj->musicHandle){
                   if(music.musicName == safeTitle){
                       return music.gith->gw.repo;
                   }
               }
           }
           else return nullptr;
       }
   
       git_repository* getNoteRepo(){
           if(E_obj.has_value()){
               return E_obj->noteHandle.first->gw.repo;
           }
           else return nullptr;
       }
   
       git_repository* getKVRepo(){
           if(E_obj.has_value()){
               return E_obj->KVHandler.first->gw.repo;
           }
           else return nullptr;
       }
   
       template<typename EDIT_ARG_TYPE>
       bool AddLine(const EDIT_ARG_TYPE& obj);
   
       bool AddLine(const UNSANITIZED& musicName, const DONT_SANITIZE& firstBar);
       
       int deleteLine(
           const EDIT_ARG_MIX& obj,
           bool skipType, 
           bool skipDetail);
   
       template<typename EDIT_ARG_TYPE> 
       int deleteLine(const EDIT_ARG_TYPE& obj);
   
       bool render(const UNSANITIZED& trackTitle, litedb& ROOTDB);
   
       void demoPlayInit(
           std::shared_ptr<audioPlayer>& player, 
           unsigned int frameBufferSize, 
           const UNSANITIZED& trackTitle);
   
       bool pushToRootDB(litedb& ROOTDB, const UNSANITIZED& trackTitleToPush);
       
       bool pushToRootDB(
           litedb& ROOTDB, 
           const UNSANITIZED& musicTitle, 
           const UNSANITIZED& musicComposer);
   
       template<typename EDIT_ARG_TYPE> 
       void getAll(std::function<void(const EDIT_ARG_TYPE& obj)> jsonCallback);
       
       template<typename EDIT_ARG_TYPE> 
       bool Undo();
       
       template<typename EDIT_ARG_TYPE> 
       bool Undo(const UNSANITIZED& musicName);
       
       template<typename EDIT_ARG_TYPE> 
       bool Redo();
   
       template<typename EDIT_ARG_TYPE> 
       bool Redo(const UNSANITIZED& musicName);
   
       template<typename EDIT_ARG_TYPE> 
       bool Go(const DONT_SANITIZE& branchName, git_oid* commitID);
   
       template<typename EDIT_ARG_TYPE> 
       DONT_SANITIZE GetLogWithJSONGraph();
       
       template<typename EDIT_ARG_TYPE> 
       DONT_SANITIZE GetLogWithJSONGraph(const UNSANITIZED& musicName);
       
       template<typename EDIT_ARG_TYPE> 
       bool UpdateLog();
   
       template<typename EDIT_ARG_TYPE> 
       bool UpdateLog(const DONT_SANITIZE& branchName);
   
       template<typename EDIT_ARG_TYPE> 
       DiffResult GetDiff(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
   
       nj& operator[](const DONT_SANITIZE& key);
       
       DONT_SANITIZE DESTROY_PROJECT();
   
       bool ConfigNewMusic(const UNSANITIZED& NewMusicName, 
                           const UNSANITIZED& composer,
                           const fs::path& musicPath,
                           const DONT_SANITIZE& firstBar = "0");
   
       bool Open(const fs::path& projectPath);
   
       editorObject() = delete;
   
       editorObject(const DONT_SANITIZE &auth_name, const DONT_SANITIZE &auth_email){
           E_obj.emplace(auth_name, auth_email);
       }
   
       ~editorObject() = default;
   };
   
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_NOTE>();
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MIX>();
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_KEY_VALUE>();
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);
   
   template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_NOTE>(const EDIT_ARG_NOTE& obj);
   template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_MIX>(const EDIT_ARG_MIX& obj);
   template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_KEY_VALUE>(const EDIT_ARG_KEY_VALUE& obj);
   template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_MUSIC>(const EDIT_ARG_MUSIC& obj);
   
   template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_NOTE>();
   template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_MIX>();
   template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_KEY_VALUE>();
   template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj);
   
   template<> PDJE_API int editorObject::deleteLine<EDIT_ARG_NOTE>(const EDIT_ARG_NOTE& obj);
   template<> PDJE_API int editorObject::deleteLine<EDIT_ARG_KEY_VALUE>(const EDIT_ARG_KEY_VALUE& obj);
   template<> PDJE_API int editorObject::deleteLine<EDIT_ARG_MUSIC>(const EDIT_ARG_MUSIC& obj);
   
   template<> PDJE_API void editorObject::getAll<EDIT_ARG_NOTE>(std::function<void(const EDIT_ARG_NOTE& obj)> jsonCallback);
   template<> PDJE_API void editorObject::getAll<EDIT_ARG_MIX>(std::function<void(const EDIT_ARG_MIX& obj)> jsonCallback);
   template<> PDJE_API void editorObject::getAll<EDIT_ARG_KEY_VALUE>(std::function<void(const EDIT_ARG_KEY_VALUE& obj)> jsonCallback);
   template<> PDJE_API void editorObject::getAll<EDIT_ARG_MUSIC>(std::function<void(const EDIT_ARG_MUSIC& obj)> jsonCallback);
   
   template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_NOTE>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
   template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_MIX>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
   template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_KEY_VALUE>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
   template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_MUSIC>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
   
   template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_NOTE>();
   template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_MIX>();
   template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
   template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_MUSIC>();
   
   template<> PDJE_API bool editorObject::Go<EDIT_ARG_NOTE>(const DONT_SANITIZE& branchName, git_oid* commitID);
   template<> PDJE_API bool editorObject::Go<EDIT_ARG_MIX>(const DONT_SANITIZE& branchName, git_oid* commitID);
   template<> PDJE_API bool editorObject::Go<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE& branchName, git_oid* commitID);
   template<> PDJE_API bool editorObject::Go<EDIT_ARG_MUSIC>(const DONT_SANITIZE& branchName, git_oid* commitID);
   
   template<> PDJE_API bool editorObject::Redo<EDIT_ARG_NOTE>();
   template<> PDJE_API bool editorObject::Redo<EDIT_ARG_MIX>();
   template<> PDJE_API bool editorObject::Redo<EDIT_ARG_KEY_VALUE>();
   template<> PDJE_API bool editorObject::Redo<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);
   
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_NOTE>();
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MIX>();
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_KEY_VALUE>();
   template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);
   
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_NOTE>();
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_NOTE>(const DONT_SANITIZE& branchName);
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MIX>();
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MIX>(const DONT_SANITIZE& branchName);
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>();
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE& branchName);
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MUSIC>();
   template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);
