#include "editorObject.hpp"

template<>
PDJE_API
void
editorObject::getAll(std::function<void(const EDIT_ARG_KEY_VALUE& obj)> jsonCallback)
{
    E_obj->KVHandler.second.getAll(jsonCallback);
}

template<>
PDJE_API
void
editorObject::getAll(std::function<void(const EDIT_ARG_MIX& obj)> jsonCallback)
{
    E_obj->mixHandle.second.getAll(jsonCallback);
}

template<>
PDJE_API
void
editorObject::getAll(std::function<void(const EDIT_ARG_NOTE& obj)> jsonCallback)
{
    E_obj->noteHandle.second.getAll(jsonCallback);
}

template<>
PDJE_API
void
editorObject::getAll(std::function<void(const EDIT_ARG_MUSIC& obj)> jsonCallback)
{
    for(auto& i :  E_obj->musicHandle){
        i.jsonh.getAll(jsonCallback);
    }
}

