#include "editorObject.hpp"

template<>
PDJE_API
bool
editorObject::Redo<EDIT_ARG_MIX>()
{
    return E_obj->mixHandle.first->Redo(); 
}

template<>
PDJE_API
bool
editorObject::Redo<EDIT_ARG_NOTE>()
{
    return E_obj->noteHandle.first->Redo();
}
template<>
PDJE_API
bool
editorObject::Redo<EDIT_ARG_MUSIC>(const std::u8string& musicName)
{
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == musicName){
            return i.gith->Redo();
        }
    }
    return false;
}
template<>
PDJE_API
bool
editorObject::Redo<EDIT_ARG_KEY_VALUE>()
{
    return E_obj->KVHandler.first->Redo();
}
