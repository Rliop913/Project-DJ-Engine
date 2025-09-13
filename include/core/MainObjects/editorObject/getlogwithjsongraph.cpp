#include "editorObject.hpp"

template <>
PDJE_API DONT_SANITIZE
editorObject::GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>()
{
    return E_obj->KVHandler.first->GetLogWithJSONGraph();
}

template <>
PDJE_API DONT_SANITIZE
editorObject::GetLogWithJSONGraph<EDIT_ARG_MIX>()
{
    return E_obj->mixHandle.first->GetLogWithJSONGraph();
}

template <>
PDJE_API DONT_SANITIZE
editorObject::GetLogWithJSONGraph<EDIT_ARG_NOTE>()
{
    return E_obj->noteHandle.first->GetLogWithJSONGraph();
}

template <>
PDJE_API DONT_SANITIZE
editorObject::GetLogWithJSONGraph<EDIT_ARG_MUSIC>(const UNSANITIZED &musicName)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
    if (!safeMus) {
        critlog("Music name is not sanitized from editorObject "
                "GetLogWithJSONGraph. musicName: ");
        critlog(musicName);
        return DONT_SANITIZE();
    }
    for (auto &i : E_obj->musicHandle) {
        if (i.musicName == safeMus) {
            return i.gith->GetLogWithJSONGraph();
        }
    }
    warnlog("music is not exists. from editorObject GetLogWithJSONGraph(Music "
            "obj). musicName:");
    warnlog(musicName);
    return DONT_SANITIZE();
}
