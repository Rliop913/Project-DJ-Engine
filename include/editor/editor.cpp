#include "editor.hpp"
#include <filesystem>

namespace fs = std::filesystem;

bool
PDJE_Editor::openProject(const fs::path& projectPath)
{
    pt = projectPath;
    mixp = pt / "Mixes";
    notep = pt / "Notes";
    musicp = pt / "Musics";
    kvp = pt / "KeyValues";
    
    if(!fs::exists(pt) || !fs::is_directory(pt)){
        fs::create_directory(pt);
        fs::create_directory(mixp);
        fs::create_directory(notep);
        fs::create_directory(musicp);
        fs::create_directory(kvp);
        if(
            !fs::exists(pt)     || !fs::is_directory(pt)    ||
            !fs::exists(mixp)   || !fs::is_directory(mixp)  ||
            !fs::exists(notep)  || !fs::is_directory(notep) ||
            !fs::exists(kvp)    || !fs::is_directory(kvp)   ||
            !fs::exists(musicp) || !fs::is_directory(musicp)
        ){ return false; }
    }
    if( !mixHandle.first->Open(mixp)    || !mixHandle.second.load(mixp) ||
        !KVHandler.first->Open(kvp)     || !KVHandler.second.load(kvp)  ||
        !noteHandle.first->Open(notep)  || !noteHandle.second.load(notep))
        { return false; }

    for(const auto& musicSubpath : fs::directory_iterator(musicp)){
        if(fs::is_directory(musicSubpath)){
            
            musicHandle.emplace_back(name, email);
            musicHandle.back().musicName = musicSubpath.path().filename().u8string();
            if( !musicHandle.back().gith->Open(musicSubpath.path()) ||
                !musicHandle.back().jsonh.load(musicSubpath.path()) ){
                    return false;
                }
        }
    }
    return true;
    
}

bool
PDJE_Editor::AddMusicConfig(const std::u8string& NewMusicName)
{
    auto newpath = musicp / fs::path(NewMusicName);
    if(fs::exists(newpath)){
        return false;
    }
    else{
        fs::create_directory(newpath);
        if(fs::exists(newpath)){
            musicHandle.emplace_back(name, email);
            musicHandle.back().musicName = NewMusicName;
            if( !musicHandle.back().gith->Open(newpath) ||
                !musicHandle.back().jsonh.load(newpath) ){
                    return false;
                }
            else return true;
        }
    }
    return false;
}