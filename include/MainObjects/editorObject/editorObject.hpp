#pragma once

#include <optional>
#include <filesystem>
#include "editor.hpp"

struct EDIT_ARG_MUSIC{
    std::string musicName;
    MusicArgs arg;
};

using EDIT_ARG_NOTE = NoteArgs;
using EDIT_ARG_MIX  = MixArgs;
using EDIT_ARG_KEY_VALUE = KEY_VALUE;


class editorObject {
private:
    
    fs::path mixFilePath;
    fs::path noteFilePath;
    fs::path kvFilePath;
    fs::path musicFileRootPath;
    PDJE_Editor E_obj;
public:

    template<typename EDIT_ARG_TYPE>
    bool AddLine(const EDIT_ARG_TYPE& obj);
    
    template<typename EDIT_ARG_TYPE> 
    int deleteLine(
        const EDIT_ARG_TYPE& obj,
        bool skipType = false, 
        bool skipDetail = false);//todo-impl

    template<typename EDIT_ARG_TYPE> 
    int deleteLine(const EDIT_ARG_TYPE& obj);//todo-impl

    template<typename EDIT_ARG_TYPE>
    bool render();//todo-impl

    template<typename EDIT_ARG_TYPE> 
    void getAll(std::function<void(const EDIT_ARG_TYPE& obj)> jsonCallback);//todo-impl
    
    template<typename EDIT_ARG_TYPE> 
    bool Undo();//todo-impl
    
    template<typename EDIT_ARG_TYPE> 
    bool Redo();//todo-impl

    template<typename EDIT_ARG_TYPE> 
    bool Go(const std::string& branchName, git_oid* commitID);//todo-impl

    template<typename EDIT_ARG_TYPE> 
    std::string GetLogWithJSONGraph();//todo-impl
    
    template<typename EDIT_ARG_TYPE> 
    bool UpdateLog();//todo-impl

    template<typename EDIT_ARG_TYPE> 
    bool UpdateLog(const std::string& branchName);//todo-impl

    template<typename EDIT_ARG_TYPE> 
    DiffResult GetDiff(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);//todo-impl

    nj operator[](const std::string& key);//todo-impl
    
    ///WARNING!!! THERE IS NO TURNING BACK
    bool DESTROY_PROJECT();//todo-impl

    bool ConfigNewMusic(const std::string& NewMusicName){
        return E_obj.AddMusicConfig(NewMusicName);
    }
    bool Open(const std::string& projectPath){
        auto pt = fs::path(projectPath);
        mixFilePath = pt / "Mixes" / "mixmetadata.PDJE";
        noteFilePath = pt / "Notes" / "notemetadata.PDJE";
        kvFilePath = pt / "KeyValues" / "keyvaluemetadata.PDJE";
        musicFileRootPath = pt / "Musics";
        return E_obj.openProject(projectPath);
    }
    editorObject() = delete;
    editorObject(const std::string &auth_name, const std::string &auth_email):
    E_obj(auth_name, auth_email) {}
    ~editorObject();
};