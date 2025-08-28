#include "editorObject.hpp"

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>()
{
    return E_obj->KVHandler.first->UpdateLog();
}

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE &branchName)
{

    return E_obj->KVHandler.first->UpdateLog(branchName);
}

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_MIX>()
{
    return E_obj->mixHandle.first->UpdateLog();
}

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_MIX>(const DONT_SANITIZE &branchName)
{
    return E_obj->mixHandle.first->UpdateLog(branchName);
}

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_NOTE>()
{
    return E_obj->noteHandle.first->UpdateLog();
}

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_NOTE>(const DONT_SANITIZE &branchName)
{
    return E_obj->noteHandle.first->UpdateLog(branchName);
}

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_MUSIC>()
{
    for (auto &i : E_obj->musicHandle) {
        if (!i.gith->UpdateLog()) {
            critlog("failed to update log. from editorObject UpdateLog(Music "
                    "obj). musicName:");
            critlog(i.musicName);
            return false;
        }
    };
    return true;
}

template <>
PDJE_API bool
editorObject::UpdateLog<EDIT_ARG_MUSIC>(const UNSANITIZED &musicName)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
    if (!safeMus) {
        critlog("Music name is not sanitized from editorObject UpdateLog. "
                "musicName: ");
        critlog(musicName);
        return false;
    }
    for (auto &i : E_obj->musicHandle) {
        if (i.musicName == safeMus) {
            return i.gith->UpdateLog();
        }
    };
    warnlog("music is not exists. from editorObject UpdateLog(Music obj). "
            "musicName:");
    warnlog(musicName);
    return false;
}
