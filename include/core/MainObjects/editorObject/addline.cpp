#include "MusicJsonHelper.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "editorObject.hpp"

template <>
PDJE_API bool
editorObject::AddLine(const EDIT_ARG_MUSIC &obj)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(obj.musicName);
    if (!safeMus) {
        critlog("Music name is not sanitized from editorObject Addline. "
                "musicName: ");
        critlog(obj.musicName);
        return false;
    }

    for (auto &i : edit_core->musicHandle) {
        if (GetTitle(*i.handle->GetJson()) == safeMus.value()) {
            return i.handle->WriteData(obj.arg);
        }
    }
    warnlog("music is not exists. from editorObject AddLine(Music obj)");
    return false;
}

template <>
PDJE_API bool
editorObject::AddLine(const EDIT_ARG_MIX &obj)
{
    if (obj.type == TypeEnum::LOAD) {
        auto safeObj = obj;
        auto first   = PDJE_Name_Sanitizer::sanitizeFileName(safeObj.first);
        auto second  = PDJE_Name_Sanitizer::sanitizeFileName(safeObj.second);
        if (!first || !second) {
            critlog(
                "Mix name is not sanitized from editorObject Addline. first: ");
            critlog(obj.first);
            critlog("second: ");
            critlog(obj.second);
            return false;
        }
        safeObj.first  = first.value();
        safeObj.second = second.value();
        if (!edit_core->mixHandle->WriteData(safeObj)) {
            critlog("failed to Write Mix args. First & Second: ");
            critlog(first.value());
            critlog(second.value());
            return false;
        }

    } else {
        if (!edit_core->mixHandle->WriteData(obj)) {
            critlog("Mix add failed from editorObject Addline. obj: ");
            critlog(obj.first);
            critlog("second: ");
            critlog(obj.second);
            return false;
        }
    }
    return true;
}

template <>
PDJE_API bool
editorObject::AddLine(const EDIT_ARG_NOTE &obj)
{

    if (!edit_core->noteHandle->WriteData(obj)) {
        critlog("Note add failed from editorObject Addline. obj: ");
        critlog(obj.first);
        critlog("second: ");
        critlog(obj.second);
        critlog("third: ");
        critlog(obj.third);
        critlog("railID: ");
        critlog(obj.railID);

        return false;
    }
    return true;
}

template <>
PDJE_API bool
editorObject::AddLine(const EDIT_ARG_KEY_VALUE &obj)
{

    if (!edit_core->KVHandle->WriteData(obj)) {
        critlog("KV add failed from editorObject Addline. obj: ");
        critlog(std::string(obj.first));
        critlog("second: ");
        critlog(std::string(obj.second));

        return false;
    }
    return true;
}

bool
editorObject::AddLine(const UNSANITIZED   &musicName,
                      const DONT_SANITIZE &firstBeat)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(musicName);
    if (!safeMus) {
        critlog("Music name is not sanitized from editorObject Addline. "
                "musicName: ");
        critlog(musicName);
        return false;
    }
    for (auto &i : edit_core->musicHandle) {
        if (GetTitle(*i.handle->GetJson()) == safeMus) {
            SetFirstBeat(*i.handle->GetJson(), firstBeat);
        }
    }
    warnlog(
        "music is not exists. from editorObject AddLine(musicName, firstBeat)");
    return false;
}