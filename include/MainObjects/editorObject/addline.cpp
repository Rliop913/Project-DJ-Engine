#include "editorObject.hpp"

template<>
bool
editorObject::AddLine(const EDIT_ARG_MUSIC& obj)
{
    
    for(auto& i : E_obj.musicHandle){
        if(i.musicName == obj.musicName){
            i.jsonh.add(obj.arg);
            i.jsonh.save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").string());
            i.gith.Save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").string(), GitWrapper::GenTimeStamp());
            return true;
        }
    }
    return false;
}

template<>
bool
editorObject::AddLine(const EDIT_ARG_MIX& obj)
{

    if(!E_obj.mixHandle.second.add(obj)) return false;
    if(!E_obj.mixHandle.second.save(mixFilePath.string())) return false;
    if(!E_obj.mixHandle.first.Save(mixFilePath.string(), GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
bool
editorObject::AddLine(const EDIT_ARG_NOTE& obj)
{

    if(!E_obj.noteHandle.second.add(obj)) return false;
    if(!E_obj.noteHandle.second.save(noteFilePath.string())) return false;
    if(!E_obj.noteHandle.first.Save(noteFilePath.string(), GitWrapper::GenTimeStamp())) return false;
    return true;
}


template<>
bool
editorObject::AddLine(const EDIT_ARG_KEY_VALUE& obj)
{

    if(!E_obj.KVHandler.second.add(obj)) return false;
    if(!E_obj.KVHandler.second.save(kvFilePath.string())) return false;
    if(!E_obj.KVHandler.first.Save(kvFilePath.string(), GitWrapper::GenTimeStamp())) return false;
    return true;
}

