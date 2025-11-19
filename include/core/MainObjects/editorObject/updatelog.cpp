#include "editorObject.hpp"

template <>
PDJE_API void
editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>()
{
    edit_core->KVHandle->UpdateLogs();
}

template <>
PDJE_API void
editorObject::UpdateLog<EDIT_ARG_MIX>()
{
    edit_core->mixHandle->UpdateLogs();
}

template <>
PDJE_API void
editorObject::UpdateLog<EDIT_ARG_NOTE>()
{
    edit_core->noteHandle->UpdateLogs();
}

template <>
PDJE_API void
editorObject::UpdateLog<EDIT_ARG_MUSIC>()
{
    for (auto &i : edit_core->musicHandle) {
        i.handle->UpdateLogs();
    }
}