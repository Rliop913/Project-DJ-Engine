#include "editorObject.hpp"

template<>
std::string
editorObject::GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>()
{
    return E_obj->KVHandler.first.GetLogWithJSONGraph();
}

template<>
std::string
editorObject::GetLogWithJSONGraph<EDIT_ARG_MIX>()
{
    return E_obj->mixHandle.first.GetLogWithJSONGraph();
}

template<>
std::string
editorObject::GetLogWithJSONGraph<EDIT_ARG_NOTE>()
{
    return E_obj->noteHandle.first.GetLogWithJSONGraph();
}

template<>
std::string
editorObject::GetLogWithJSONGraph<EDIT_ARG_MUSIC>(const std::string& musicName)
{
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == musicName){
            return i.gith.GetLogWithJSONGraph();
        }
    }
    return std::string();
}




