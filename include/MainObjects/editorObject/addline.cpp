#include "editorObject.hpp"

template<>
bool
editorObject::AddLine(const EDIT_ARG_MUSIC& obj)
{
    
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == obj.musicName){
            i.jsonh.add(obj.arg);
            return DefaultSaveFuntion<EDIT_ARG_MUSIC>(i, obj);
        }
    }
    return false;
}

template<>
bool
editorObject::AddLine(const EDIT_ARG_MIX& obj)
{

    if(!E_obj->mixHandle.second.add(obj)) return false;
    return DefaultSaveFuntion<EDIT_ARG_MIX>();
}

template<>
bool
editorObject::AddLine(const EDIT_ARG_NOTE& obj)
{

    if(!E_obj->noteHandle.second.add(obj)) return false;
    return DefaultSaveFuntion<EDIT_ARG_NOTE>();
}


template<>
bool
editorObject::AddLine(const EDIT_ARG_KEY_VALUE& obj)
{

    if(!E_obj->KVHandler.second.add(obj)) return false;
    return DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>();
}

