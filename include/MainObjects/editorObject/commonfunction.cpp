#include "editorObject.hpp"



template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MIX>()
{
    if(!E_obj->mixHandle.second.save(mixFilePath.string())) return false;
    if(!E_obj->mixHandle.first->Save(mixFilePath.string(), GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_NOTE>()
{
    if(!E_obj->noteHandle.second.save(noteFilePath.string())) return false;
    if(!E_obj->noteHandle.first->Save(noteFilePath.string(), GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>()
{
    if(!E_obj->KVHandler.second.save(kvFilePath.string())) return false;
    if(!E_obj->KVHandler.first->Save(kvFilePath.string(), GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj)
{
    if(!i.jsonh.save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").string())) return false;
    if(!i.gith.Save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").string(), GitWrapper::GenTimeStamp())) return false;
    return true;
}