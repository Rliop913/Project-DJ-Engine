#include "editorObject.hpp"

template<> 
PDJE_API
bool
editorObject::AddLine(const EDIT_ARG_MUSIC& obj)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(obj.musicName);
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == safeMus){
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
    if(obj.type == TypeEnum::LOAD){
        auto safeObj = obj;
        auto first = PDJE_Name_Sanitizer::sanitizeFileName(safeObj.first);
        auto second = PDJE_Name_Sanitizer::sanitizeFileName(safeObj.second);
        if(!first || !second){
            return false;
        }
        safeObj.first = first.value();
        safeObj.second = second.value();
        if(!E_obj->mixHandle.second.add(safeObj)) return false;
    }
    else{
        if(!E_obj->mixHandle.second.add(obj)) return false;
    }
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

#include <iostream>
bool 
editorObject::AddLine(const std::string& musicName, const std::string& firstBar)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == safeMus){
            i.jsonh["FIRST_BAR"] = firstBar;
            std::cout << "DEBUGLINE: addline.cpp:56   " << firstBar << std::endl;
            return true;
        }
    }
    return false;
}