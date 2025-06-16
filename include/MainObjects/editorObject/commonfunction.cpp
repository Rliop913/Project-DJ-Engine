#include "editorObject.hpp"



template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MIX>()
{
    if(!E_obj->mixHandle.second.save(mixFilePath.generic_string())) return false;
    if(!E_obj->mixHandle.first->Save("mixmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_NOTE>()
{
    if(!E_obj->noteHandle.second.save(noteFilePath.generic_string())) return false;
    if(!E_obj->noteHandle.first->Save("notemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>()
{
    if(!E_obj->KVHandler.second.save(kvFilePath.generic_string())) return false;
    if(!E_obj->KVHandler.first->Save("keyvaluemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj)
{
    if(!i.jsonh.save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").generic_string())) return false;
    if(!i.gith.Save("musicmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
    return true;
}