#pragma once


#include "jsonWrapper.hpp"
#include "gitWrapper.hpp"
#include "dbRoot.hpp"
#include <filesystem>

#include "PDJE_EXPORT_SETTER.hpp"

class PDJE_API PDJE_Editor{
private:
    fs::path pt;
    fs::path mixp;
    fs::path notep;
    fs::path musicp;
    fs::path kvp;
    std::string name;
    std::string email;
public:
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<MIX_W>> mixHandle;
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<NOTE_W>> noteHandle;
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<KV_W>> KVHandler;
    struct MusicHandleStruct{
        std::unique_ptr<PDJE_GitHandler> gith;
        PDJE_JSONHandler<MUSIC_W> jsonh;
        std::u8string musicName;
         MusicHandleStruct() = delete;
        MusicHandleStruct(const MusicHandleStruct&) = delete;
        MusicHandleStruct& operator=(const MusicHandleStruct&) = delete;
        MusicHandleStruct(MusicHandleStruct&&) noexcept = default;
        MusicHandleStruct& operator=(MusicHandleStruct&&) noexcept = default;
        
        MusicHandleStruct(const std::string &auth_name, const std::string &auth_email):
        jsonh(){
            gith = std::make_unique<PDJE_GitHandler>(auth_name, auth_email);
        }
    };
    std::vector<MusicHandleStruct> musicHandle;
    bool AddMusicConfig(const std::u8string& NewMusicName);
    bool openProject(const std::u8string& projectPath);

    PDJE_Editor(const std::string &auth_name, const std::string &auth_email):
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