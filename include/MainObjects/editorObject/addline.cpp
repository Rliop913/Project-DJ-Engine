#include "editorObject.hpp"

template<> 
PDJE_API
bool
editorObject::AddLine(const EDIT_ARG_MUSIC& obj)
{
    
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == TO_USTR(obj.musicName)){
            i.jsonh.add(obj.arg);
            return DefaultSaveFuntion<EDIT_ARG_MUSIC>(i, obj);
        }
    }
    return false;
}

template<> 
PDJE_API
bool
editorObject::AddLine(const EDIT_ARG_MIX& obj)
{

    if(!E_obj->mixHandle.second.add(obj)) return false;
    return DefaultSaveFuntion<EDIT_ARG_MIX>();
}

template<> 
PDJE_API
bool
editorObject::AddLine(const EDIT_ARG_NOTE& obj)
{

    if(!E_obj->noteHandle.second.add(obj)) return false;
    return DefaultSaveFuntion<EDIT_ARG_NOTE>();
}


template<> 
PDJE_API
bool
editorObject::AddLine(const EDIT_ARG_KEY_VALUE& obj)
{

    if(!E_obj->KVHandler.second.add(obj)) return false;
    return DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>();
}

bool 
editorObject::AddLine(const std::u8string& musicName, const std::string& firstBar)
{
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == musicName){
            i.jsonh["FIRST_BAR"] = firstBar;
            return true;
        }
    }
    return false;
}