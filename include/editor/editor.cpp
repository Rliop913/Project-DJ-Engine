#include "editor.hpp"
#include <filesystem>

namespace fs = std::filesystem;

bool
PDJE_Editor::openProject(const std::string& projectPath)
{
    pt = fs::path(projectPath);
    mixp = pt / "Mixes";
    notep = pt / "Notes";
    musicp = pt / "Musics";
    
    if(!fs::exists(pt) || !fs::is_directory(pt)){
        fs::create_directory(pt);
        fs::create_directory(mixp);
        fs::create_directory(notep);
        fs::create_directory(musicp);
        if(
            !fs::exists(pt)     || !fs::is_directory(pt)    ||
            !fs::exists(mixp)   || !fs::is_directory(mixp)  ||
            !fs::exists(notep)  || !fs::is_directory(notep) ||
            !fs::exists(musicp) || !fs::is_directory(musicp)
        ){ return false; }
    }
    if( !mixHandle.first.Open(mixp.string())|| !mixHandle.second.load(mixp.string()) ||
        !noteHandle.first.Open(notep.string()) || !noteHandle.second.load(notep.string()))
        { return false; }

    for(const auto& musicSubpath : fs::directory_iterator(musicp)){
        if(fs::is_directory(musicSubpath)){
            

            musicHandle.emplace_back(PDJE_GitHandler(name, email), PDJE_JSONHandler<MUSIC_W>());
            if( !musicHandle.back().first.Open(musicSubpath.path().string()) ||
                !musicHandle.back().second.load(musicSubpath.path().string())){
                    return false;
                }
        }
    }
    return true;
    
}

bool
PDJE_Editor::AddMusicConfig(const std::string& NewMusicName)
{
    return false;//todo - impl
}