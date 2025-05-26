#pragma once

#include <optional>
#include <filesystem>
#include "editor.hpp"

#include "tempDB.hpp"

struct EDIT_ARG_MUSIC{
    std::string musicName;
    
    MusicArgs arg;
};

using EDIT_ARG_NOTE = NoteArgs;
using EDIT_ARG_MIX  = MixArgs;
using EDIT_ARG_KEY_VALUE = KEY_VALUE;


class editorObject {
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

public:

    template<typename EDIT_ARG_TYPE>
    bool AddLine(const EDIT_ARG_TYPE& obj);

    bool AddLine(const std::string& musicName, const std::string& firstBar){
        for(auto& i : E_obj->musicHandle){
            if(i.musicName == musicName){
                i.jsonh["FIRST_BAR"] = firstBar;
                return true;
            }
        }
        return false;
    }
    
    
    int deleteLine(
        const EDIT_ARG_MIX& obj,
        bool skipType, 
        bool skipDetail);

    template<typename EDIT_ARG_TYPE> 
    int deleteLine(const EDIT_ARG_TYPE& obj);

    bool render(const std::string& trackTitle, litedb& ROOTDB);

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
    std::string DESTROY_PROJECT(){
        try
        {
            E_obj.reset();
            projectLocalDB.reset();
            auto deletedAmount = fs::remove_all(projectRoot);
            if(deletedAmount < 1){
                return "DELETED NOTHING";
            }
            else{
                return "COMPLETE";
            }
        }
        catch(const std::exception& e)
        {
            return e.what();
        }
    }

    bool ConfigNewMusic(const std::string& NewMusicName, 
                        const std::string& composer,
                        const std::string& musicPath,
                        const std::string& firstBar = "0"){
        if( E_obj->AddMusicConfig(NewMusicName)){

            E_obj->musicHandle.back().jsonh["TITLE"] = NewMusicName;
            E_obj->musicHandle.back().jsonh["COMPOSER"] = composer;
            E_obj->musicHandle.back().jsonh["PATH"] = musicPath;
            E_obj->musicHandle.back().jsonh["FIRST_BAR"] = firstBar;
            return true;
        }
        else{
            return false;
        }
    }
    bool Open(const std::string& projectPath){
        projectRoot = fs::path(projectPath);
        
        mixFilePath = projectRoot / "Mixes" / "mixmetadata.PDJE";
        noteFilePath = projectRoot / "Notes" / "notemetadata.PDJE";
        kvFilePath = projectRoot / "KeyValues" / "keyvaluemetadata.PDJE";
        musicFileRootPath = projectRoot / "Musics";
        projectLocalDB.emplace();
        
        return E_obj->openProject(projectPath) && projectLocalDB->Open(projectRoot);
    }
    editorObject() = delete;
    editorObject(const std::string &auth_name, const std::string &auth_email){
        E_obj.emplace(auth_name, auth_email);
    }
    ~editorObject();
};