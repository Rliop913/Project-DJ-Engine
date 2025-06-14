#include "editorObject.hpp"

template<>
PDJE_API
bool
editorObject::Go<EDIT_ARG_MIX>(const std::string& branchName, git_oid* commitID)
{
    return E_obj->mixHandle.first.Go(branchName, commitID);
}

template<>
PDJE_API
bool
editorObject::Go<EDIT_ARG_NOTE>(const std::string& branchName, git_oid* commitID)
{
    return E_obj->noteHandle.first.Go(branchName, commitID);
}

template<>
PDJE_API
bool
editorObject::Go<EDIT_ARG_KEY_VALUE>(const std::string& branchName, git_oid* commitID)
{
    return E_obj->KVHandler.first.Go(branchName, commitID);
}

template<>
PDJE_API
bool
editorObject::Go<EDIT_ARG_MUSIC>(const std::string& branchName, git_oid* commitID)
{
    for(auto& i : E_obj->musicHandle){
        if(i.gith.Go(branchName, commitID)) return true;
    }
    return false;
}

