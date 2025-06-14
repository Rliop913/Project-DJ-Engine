#pragma once

#include <optional>
#include <filesystem>
#include "editor.hpp"
#include "audioPlayer.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "tempDB.hpp"

struct PDJE_API EDIT_ARG_MUSIC{
    std::string musicName;
    
    MusicArgs arg;
};

using EDIT_ARG_NOTE = NoteArgs;
using EDIT_ARG_MIX  = MixArgs;
using EDIT_ARG_KEY_VALUE = KEY_VALUE;
using TITLE_COMPOSER = std::unordered_map<std::string, std::string>;

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
        const std::string& trackTitle, 
        TITLE_COMPOSER& titles);

public:
    git_repository* getMixRepo(){
        if(E_obj.has_value()){
            return E_obj->mixHandle.first.gw.repo;
        }
        else return nullptr;
    }
    git_repository* getMusicRepo(const std::string& Title){
        if(E_obj.has_value()){
            for(auto& music : E_obj->musicHandle){
                if(music.musicName == Title){
                    return music.gith.gw.repo;
                }
            }
        }
        else return nullptr;
    }

    git_repository* getNoteRepo(){
        if(E_obj.has_value()){
            return E_obj->noteHandle.first.gw.repo;
        }
        else return nullptr;
    }

    git_repository* getKVRepo(){
        if(E_obj.has_value()){
            return E_obj->KVHandler.first.gw.repo;
        }
        else return nullptr;
    }



    template<typename EDIT_ARG_TYPE>
    bool AddLine(const EDIT_ARG_TYPE& obj);

    bool AddLine(const std::string& musicName, const std::string& firstBar);
    
    
    int deleteLine(
        const EDIT_ARG_MIX& obj,
        bool skipType, 
        bool skipDetail);

    template<typename EDIT_ARG_TYPE> 
    int deleteLine(const EDIT_ARG_TYPE& obj);

    bool render(const std::string& trackTitle, litedb& ROOTDB);

    void demoPlayInit(
        std::optional<audioPlayer>& player, 
        unsigned int frameBufferSize, 
        const std::string& trackTitle);

    bool pushToRootDB(litedb& ROOTDB, const std::string& trackTitleToPush);

    bool pushToRootDB(
        litedb& ROOTDB, 
        const std::string& musicTitle, 
        const std::string& musicComposer);

    template<typename EDIT_ARG_TYPE> 
    void getAll(std::function<void(const EDIT_ARG_TYPE& obj)> jsonCallback);
    
    template<typename EDIT_ARG_TYPE> 
    bool Undo();
    
    template<typename EDIT_ARG_TYPE> 
    bool Undo(const std::string& musicName);
    
    
    template<typename EDIT_ARG_TYPE> 
    bool Redo();

    template<typename EDIT_ARG_TYPE> 
    bool Redo(const std::string& musicName);

    template<typename EDIT_ARG_TYPE> 
    bool Go(const std::string& branchName, git_oid* commitID);

    template<typename EDIT_ARG_TYPE> 
    std::string GetLogWithJSONGraph();
    
    template<typename EDIT_ARG_TYPE> 
    std::string GetLogWithJSONGraph(const std::string& musicName);
    

    template<typename EDIT_ARG_TYPE> 
    bool UpdateLog();

    template<typename EDIT_ARG_TYPE> 
    bool UpdateLog(const std::string& branchName);

    template<typename EDIT_ARG_TYPE> 
    DiffResult GetDiff(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);

    nj& operator[](const std::string& key){
        return E_obj->KVHandler.second[key];
    }
    
    ///WARNING!!! THERE IS NO TURNING BACK
    std::string DESTROY_PROJECT();

    bool ConfigNewMusic(const std::string& NewMusicName, 
                        const std::string& composer,
                        const std::string& musicPath,
                        const std::string& firstBar = "0");


    bool Open(const std::string& projectPath);

    editorObject() = delete;

    editorObject(const std::string &auth_name, const std::string &auth_email){
        E_obj.emplace(auth_name, auth_email);
    }

    ~editorObject() = default;
};