#include "MusicJsonHelper.hpp"
#include "editorObject.hpp"

template <>
PDJE_API bool
editorObject::Undo<EDIT_ARG_NOTE>()
{
    return edit_core->noteHandle->Undo();
}

template <>
PDJE_API bool
editorObject::Undo<EDIT_ARG_MIX>()
{
    return edit_core->mixHandle->Undo();
}

template <>
PDJE_API bool
editorObject::Undo<EDIT_ARG_MUSIC>(const UNSANITIZED &musicName)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
    if (!safeMus) {
        critlog(
            "Music name is not sanitized from editorObject Undo. musicName: ");
        critlog(musicName);
        return false;
    }

    for (auto &i : edit_core->musicHandle) {

        if (GetTitle(*i.handle->GetJson()) == safeMus) {
            return i.handle->Undo();
        }
    }
    warnlog(
        "music is not exists. from editorObject Undo(Music obj). musicName:");
    warnlog(musicName);

    return false;
}

template <>
PDJE_API bool
editorObject::Undo<EDIT_ARG_KEY_VALUE>()
{
    return edit_core->KVHandle->Undo();
}
