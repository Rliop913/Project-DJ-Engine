#include "editorObject.hpp"



template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MIX>()
{
    if(!E_obj->mixHandle.second.save(mixFilePath)) return false;
    if(!E_obj->mixHandle.first->Save(std::u8string(u8"mixmetadata.PDJE"), GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_NOTE>()
{
    if(!E_obj->noteHandle.second.save(noteFilePath)) return false;
    if(!E_obj->noteHandle.first->Save(std::u8string(u8"notemetadata.PDJE"), GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>()
{
    if(!E_obj->KVHandler.second.save(kvFilePath)) return false;
    if(!E_obj->KVHandler.first->Save(std::u8string(u8"keyvaluemetadata.PDJE"), GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj)
{
    if(!i.jsonh.save((musicFileRootPath / obj.musicName / u8"musicmetadata.PDJE"))) return false;
    if(!i.gith->Save(std::u8string(u8"musicmetadata.PDJE"), GitWrapper::GenTimeStamp())) return false;
    return true;
}