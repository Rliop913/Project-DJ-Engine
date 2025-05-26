#include "editorObject.hpp"

template<>
bool
editorObject::Redo<EDIT_ARG_MIX>()
{
    return E_obj->mixHandle.first.Redo(); 
}

template<>
bool
editorObject::Redo<EDIT_ARG_NOTE>()
{
    return E_obj->noteHandle.first.Redo();
}
template<>
bool
editorObject::Redo<EDIT_ARG_MUSIC>(const std::string& musicName)
{
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == musicName){
            return i.gith.Redo();
        }
    }
    return false;
}
template<>
bool
editorObject::Redo<EDIT_ARG_KEY_VALUE>()
{
    return E_obj->KVHandler.first.Redo();
}
