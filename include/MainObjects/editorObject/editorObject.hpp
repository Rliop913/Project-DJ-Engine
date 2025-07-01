#pragma once

#include <optional>
#include <filesystem>
#include "editor.hpp"
#include "audioPlayer.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
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
    bool DefaultSaveFuntion();

    template<typename EDIT_ARG_TYPE>
    bool DefaultSaveFuntion(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj);

    trackdata makeTrackData(
        const UNSANITIZED& trackTitle, 
        TITLE_COMPOSER& titles);

public:
    std::string RECENT_ERR = "";
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
        std::optional<audioPlayer>& player, 
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

    nj& operator[](const DONT_SANITIZE& key){
        return E_obj->KVHandler.second[key];
    }
    
    ///WARNING!!! THERE IS NO TURNING BACK
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