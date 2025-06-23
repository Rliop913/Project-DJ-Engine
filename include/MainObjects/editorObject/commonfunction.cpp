#include "editorObject.hpp"



template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MIX>()
{
<<<<<<< HEAD
    if(!E_obj->mixHandle.second.save(mixFilePath.generic_u8string())) return false;
    if(!E_obj->mixHandle.first->Save(u8"mixmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
=======
    if(!E_obj->mixHandle.second.save(mixFilePath.generic_string())) return false;
    if(!E_obj->mixHandle.first->Save("mixmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
>>>>>>> 567432ca7d48bb6b441e128467b86ffdba27ca3e
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_NOTE>()
{
<<<<<<< HEAD
    if(!E_obj->noteHandle.second.save(noteFilePath.generic_u8string())) return false;
    if(!E_obj->noteHandle.first->Save(u8"notemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
=======
    if(!E_obj->noteHandle.second.save(noteFilePath.generic_string())) return false;
    if(!E_obj->noteHandle.first->Save("notemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
>>>>>>> 567432ca7d48bb6b441e128467b86ffdba27ca3e
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>()
{
<<<<<<< HEAD
    if(!E_obj->KVHandler.second.save(kvFilePath.generic_u8string())) return false;
    if(!E_obj->KVHandler.first->Save(u8"keyvaluemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
=======
    if(!E_obj->KVHandler.second.save(kvFilePath.generic_string())) return false;
    if(!E_obj->KVHandler.first->Save("keyvaluemetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
>>>>>>> 567432ca7d48bb6b441e128467b86ffdba27ca3e
    return true;
}

template<>
PDJE_API
bool
editorObject::DefaultSaveFuntion<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj)
{
<<<<<<< HEAD
    if(!i.jsonh.save((musicFileRootPath / obj.musicName / u8"musicmetadata.PDJE").generic_u8string())) return false;
    if(!i.gith->Save(u8"musicmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
=======
    if(!i.jsonh.save((musicFileRootPath / obj.musicName / "musicmetadata.PDJE").generic_string())) return false;
    if(!i.gith->Save("musicmetadata.PDJE", GitWrapper::GenTimeStamp())) return false;
>>>>>>> 567432ca7d48bb6b441e128467b86ffdba27ca3e
    return true;
}