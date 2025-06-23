#include "editor.hpp"
#include <filesystem>

namespace fs = std::filesystem;

bool
PDJE_Editor::openProject(const std::u8string& projectPath)
{
    pt = fs::path(projectPath);
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
            !fs::exists(kvp)  || !fs::is_directory(kvp) ||
            !fs::exists(musicp) || !fs::is_directory(musicp)
        ){ return false; }
    }
<<<<<<< HEAD
    if( !mixHandle.first->Open(mixp.u8string())|| !mixHandle.second.load(mixp.u8string()) ||
        !KVHandler.first->Open(kvp.u8string())|| !KVHandler.second.load(kvp.u8string()) ||
        !noteHandle.first->Open(notep.u8string()) || !noteHandle.second.load(notep.u8string()))
=======
    if( !mixHandle.first->Open(mixp.string())|| !mixHandle.second.load(mixp.string()) ||
        !KVHandler.first->Open(kvp.string())|| !KVHandler.second.load(kvp.string()) ||
        !noteHandle.first->Open(notep.string()) || !noteHandle.second.load(notep.string()))
>>>>>>> 567432ca7d48bb6b441e128467b86ffdba27ca3e
        { return false; }

    for(const auto& musicSubpath : fs::directory_iterator(musicp)){
        if(fs::is_directory(musicSubpath)){
            

            musicHandle.emplace_back(name, email);
<<<<<<< HEAD
            musicHandle.back().musicName = musicSubpath.path().filename().u8string();
            if( !musicHandle.back().gith->Open(musicSubpath.path().u8string()) ||
                !musicHandle.back().jsonh.load(musicSubpath.path().u8string())){
=======
            musicHandle.back().musicName = musicSubpath.path().filename().string();
            if( !musicHandle.back().gith->Open(musicSubpath.path().string()) ||
                !musicHandle.back().jsonh.load(musicSubpath.path().string())){
>>>>>>> 567432ca7d48bb6b441e128467b86ffdba27ca3e
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
<<<<<<< HEAD
            if( !musicHandle.back().gith->Open(newpath.u8string()) ||
                !musicHandle.back().jsonh.load(newpath.u8string())){
=======
            if( !musicHandle.back().gith->Open(newpath.string()) ||
                !musicHandle.back().jsonh.load(newpath.string())){
>>>>>>> 567432ca7d48bb6b441e128467b86ffdba27ca3e
                    return false;
                }
            else return true;
        }
    }
    return false;
}