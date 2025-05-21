#pragma once


#include "jsonWrapper.hpp"
#include "gitWrapper.hpp"
#include "dbRoot.hpp"
#include <filesystem>


class PDJE_Editor{
private:
    fs::path pt;
    fs::path mixp;
    fs::path notep;
    fs::path musicp;
    fs::path kvp;
    std::string name;
    std::string email;
public:
    std::pair<PDJE_GitHandler, PDJE_JSONHandler<MIX_W>> mixHandle;
    std::pair<PDJE_GitHandler, PDJE_JSONHandler<NOTE_W>> noteHandle;
    std::pair<PDJE_GitHandler, PDJE_JSONHandler<KV_W>> KVHandler;
    struct MusicHandleStruct{
        PDJE_GitHandler gith;
        PDJE_JSONHandler<MUSIC_W> jsonh;
        std::string musicName;
        MusicHandleStruct(const std::string &auth_name, const std::string &auth_email):
        gith(auth_name, auth_email), jsonh(){}
    };
    std::vector<MusicHandleStruct> musicHandle;
    bool AddMusicConfig(const std::string& NewMusicName);
    bool openProject(const std::string& projectPath);

    PDJE_Editor(const std::string &auth_name, const std::string &auth_email):
    mixHandle{PDJE_GitHandler(auth_email, auth_name), PDJE_JSONHandler<MIX_W>{}},
    noteHandle{PDJE_GitHandler(auth_email, auth_name), PDJE_JSONHandler<NOTE_W>{}},
    KVHandler{PDJE_GitHandler(auth_email, auth_name), PDJE_JSONHandler<KV_W>{}},
    name(auth_name),
    email(auth_email)
    {}
    
    ~PDJE_Editor() = default;
};