#include "editorObject.hpp"



template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MIX>()
{
    if(!E_obj->mixHandle.second.save(mixFilePath)) return false;
    if(!E_obj->mixHandle.first->Save("mixmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_NOTE>()
{
    if(!E_obj->noteHandle.second.save(noteFilePath)) return false;
    if(!E_obj->noteHandle.first->Save("notemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>()
{
    if(!E_obj->KVHandler.second.save(kvFilePath)) return false;
    if(!E_obj->KVHandler.first->Save("keyvaluemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(obj.musicName);
    if(!safeMus){
        return false;
    }
    for(auto& target : E_obj->musicHandle){
        if(target.musicName == safeMus.value()){
            if(!i.jsonh.save((target.dataPath / "musicmetadata.PDJE"))) return false;
            if(!i.gith->Save("musicmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
        }
    }
    return true;
}