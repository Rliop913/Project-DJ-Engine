#include "editorObject.hpp"

template <>
PDJE_API bool
editorObject::DefaultSaveFunction<EDIT_ARG_MIX>()
{
    if (!E_obj->mixHandle.second.save(mixFilePath)) {
        critlog(
            "mix save failed from editorObject DefaultSaveFunction. path: ");
        critlog(mixFilePath.generic_string());
        return false;
    }
    if (!E_obj->mixHandle.first->Save(
                                      GitWrapper::GenTimeStamp())) {
        critlog("mix git save failed from editorObject DefaultSaveFunction. "
                "path: ");
        critlog(mixFilePath.generic_string());
        return false;
    }
    return true;
}

template <>
PDJE_API bool
editorObject::DefaultSaveFunction<EDIT_ARG_NOTE>()
{
    if (!E_obj->noteHandle.second.save(noteFilePath)) {
        critlog(
            "note save failed from editorObject DefaultSaveFunction. path: ");
        critlog(noteFilePath.generic_string());
        return false;
    }
    if (!E_obj->noteHandle.first->Save(
                                       GitWrapper::GenTimeStamp())) {
        critlog("note git save failed from editorObject DefaultSaveFunction. "
                "path: ");
        critlog(noteFilePath.generic_string());
        return false;
    }
    return true;
}

template <>
PDJE_API bool
editorObject::DefaultSaveFunction<EDIT_ARG_KEY_VALUE>()
{
    if (!E_obj->KVHandler.second.save(kvFilePath)) {
        critlog("KV save failed from editorObject DefaultSaveFunction. path: ");
        critlog(kvFilePath.generic_string());
        return false;
    }
    if (!E_obj->KVHandler.first->Save(
                                      GitWrapper::GenTimeStamp())) {
        critlog(
            "KV git save failed from editorObject DefaultSaveFunction. path: ");
        critlog(kvFilePath.generic_string());
        return false;
    }
    return true;
}

template <>
PDJE_API bool
editorObject::DefaultSaveFunction<EDIT_ARG_MUSIC>(
    PDJE_Editor::MusicHandleStruct &i, const EDIT_ARG_MUSIC &obj)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(obj.musicName);
    if (!safeMus) {
        critlog("Music name is not sanitized from editorObject "
                "DefaultSaveFunction. musicName: ");
        critlog(obj.musicName);
        return false;
    }
    for (auto &target : E_obj->musicHandle) {
        if (target.musicName == safeMus.value()) {
            if (!i.jsonh.save((target.dataPath / "musicmetadata.PDJE"))) {
                critlog("music save failed from editorObject "
                        "DefaultSaveFunction. path: ");
                fs::path logPath = (target.dataPath / "musicmetadata.PDJE");
                critlog(logPath.generic_string());
                return false;
            }
            if (!i.gith->Save(
                              GitWrapper::GenTimeStamp())) {
                critlog("music git save failed from editorObject "
                        "DefaultSaveFunction. path: ");
                fs::path logPath = (target.dataPath / "musicmetadata.PDJE");
                critlog(logPath.generic_string());
                return false;
            }
        }
    }
    return true;
}