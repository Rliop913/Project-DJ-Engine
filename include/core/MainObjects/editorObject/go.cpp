#include "editorObject.hpp"

template <>
PDJE_API bool
editorObject::Go<EDIT_ARG_MIX>(const DONT_SANITIZE &commitOID)
{
    return edit_core->mixHandle->Go(commitOID);
}

template <>
PDJE_API bool
editorObject::Go<EDIT_ARG_NOTE>(const DONT_SANITIZE &commitOID)
{
    return edit_core->noteHandle->Go(commitOID);
}

template <>
PDJE_API bool
editorObject::Go<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE &commitOID)
{
    return edit_core->KVHandle->Go(commitOID);
}

template <>
PDJE_API bool
editorObject::Go<EDIT_ARG_MUSIC>(const DONT_SANITIZE &commitOID)
{
    for (auto &i : edit_core->musicHandle) {
        if (i.handle->Go(commitOID)) {
            return true;
        }
    }
    warnlog("cannot find music. from editorObject Go(Music obj)");
    return false;
}
