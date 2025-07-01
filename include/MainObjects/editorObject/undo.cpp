#include "editorObject.hpp"

template<>
PDJE_API
bool
editorObject::Undo<EDIT_ARG_NOTE>()
{
    return E_obj->noteHandle.first->Undo();
}

template<>
PDJE_API
bool
editorObject::Undo<EDIT_ARG_MIX>()
{
    return E_obj->mixHandle.first->Undo();
}

template<>
PDJE_API
bool
editorObject::Undo<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
    if(!safeMus){
        return false;
    }

    for(auto& i : E_obj->musicHandle){
        if(i.musicName == safeMus){
            return i.gith->Undo();
        }
    }
    return false;
}

template<>
PDJE_API
bool
editorObject::Undo<EDIT_ARG_KEY_VALUE>()
{
    return E_obj->KVHandler.first->Undo();
}
