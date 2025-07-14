#pragma once


#include "jsonWrapper.hpp"
#include "gitWrapper.hpp"
#include "dbRoot.hpp"
#include <filesystem>

#include "PDJE_EXPORT_SETTER.hpp"
#include "fileNameSanitizer.hpp"

class PDJE_API PDJE_Editor{
private:
    fs::path pt;
    fs::path mixp;
    fs::path notep;
    fs::path musicp;
    fs::path kvp;
    DONT_SANITIZE name;
    DONT_SANITIZE email;
public:
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<MIX_W>> mixHandle;
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<NOTE_W>> noteHandle;
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<KV_W>> KVHandler;
    struct MusicHandleStruct{
        std::unique_ptr<PDJE_GitHandler> gith;
        PDJE_JSONHandler<MUSIC_W> jsonh;
        SANITIZED musicName;
        fs::path dataPath;
        MusicHandleStruct() = delete;
        MusicHandleStruct(const MusicHandleStruct&) = delete;
        // MusicHandleStruct& operator=(const MusicHandleStruct&) = delete;
        MusicHandleStruct(MusicHandleStruct&&) noexcept = default;
        MusicHandleStruct& operator=(MusicHandleStruct&&) noexcept = default;
        
        MusicHandleStruct(const DONT_SANITIZE& auth_name, const DONT_SANITIZE& auth_email):
        jsonh(){
            gith = std::make_unique<PDJE_GitHandler>(auth_name, auth_email);
        }
    };
    std::vector<MusicHandleStruct> musicHandle;
    bool AddMusicConfig(const SANITIZED& NewMusicName, fs::path& DataPath);
    bool openProject(const fs::path& projectPath);

    PDJE_Editor(const DONT_SANITIZE& auth_name, const DONT_SANITIZE& auth_email):
    mixHandle{
        std::make_unique<PDJE_GitHandler>(auth_name, auth_email), 
        PDJE_JSONHandler<MIX_W>{}},
    noteHandle{
        std::make_unique<PDJE_GitHandler>(auth_name, auth_email), 
        PDJE_JSONHandler<NOTE_W>{}},
    KVHandler{
        std::make_unique<PDJE_GitHandler>(auth_name, auth_email),  
        PDJE_JSONHandler<KV_W>{}},
    name(auth_name),
    email(auth_email)
    {}
    ~PDJE_Editor() = default;
};