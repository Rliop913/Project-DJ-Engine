#include "editorObject.hpp"

template<>
bool
editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>()
{
    return E_obj->KVHandler.first.UpdateLog();
}

template<>
bool
editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>(const std::string& branchName)
{
    return E_obj->KVHandler.first.UpdateLog(branchName);
}


template<>
bool
editorObject::UpdateLog<EDIT_ARG_MIX>()
{
    return E_obj->mixHandle.first.UpdateLog();
}

template<>
bool
editorObject::UpdateLog<EDIT_ARG_MIX>(const std::string& branchName)
{
    return E_obj->mixHandle.first.UpdateLog(branchName);
}

template<>
bool
editorObject::UpdateLog<EDIT_ARG_NOTE>()
{
    return E_obj->noteHandle.first.UpdateLog();
}

template<>
bool
editorObject::UpdateLog<EDIT_ARG_NOTE>(const std::string& branchName)
{
    return E_obj->noteHandle.first.UpdateLog(branchName);
}


template<>
bool
editorObject::UpdateLog<EDIT_ARG_MUSIC>()
{
    for(auto& i : E_obj->musicHandle){
        if(!i.gith.UpdateLog()) return false;
    };
}

template<>
bool
editorObject::UpdateLog<EDIT_ARG_MUSIC>(const std::string& musicName)
{
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == musicName){
            return i.gith.UpdateLog();
        }
    };
    return false;
}
