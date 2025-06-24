#include "editorObject.hpp"



trackdata
editorObject::makeTrackData(
    const std::u8string& trackTitle, 
    std::unordered_map<std::u8string, std::u8string>& titles)
{
    trackdata td;
    auto mixRendered = E_obj->mixHandle.second.render();
    auto mixData = mixRendered->Wp->getDatas();
    
    for(unsigned long long i=0; i < mixData.size();++i){
        if(mixData[i].getType() == TypeEnum::LOAD){
            // auto first = std::string(mixData[i].getFirst().cStr());
            std::u8string first, second;
            first.resize(mixData[i].getFirst().size());
            second.resize(mixData[i].getSecond().size());
            std::transform(
                mixData[i].getFirst().begin(), mixData[i].getFirst().end(),
                first.begin(),
                [](char c){return static_cast<char8_t>(c);}
            );
            std::transform(
                mixData[i].getSecond().begin(), mixData[i].getSecond().end(),
                second.begin(),
                [](char c){return static_cast<char8_t>(c);}
            );
            titles.insert(std::pair(first, second));
        }
    }
    
    td.trackTitle = trackTitle;
    td.mixBinary = mixRendered->out();
    td.noteBinary = E_obj->noteHandle.second.render()->out();
    for(auto& i : titles){
        td.cachedMixList+= (i.first + u8",");
    }
    if(!titles.empty()){
        td.cachedMixList.pop_back();
    }
    return std::move(td);
}

void 
editorObject::demoPlayInit(
    std::optional<audioPlayer>& player, 
    unsigned int frameBufferSize, const std::u8string& trackTitle)
{
    if(player.has_value()){
        player.reset();
    }
    trackdata tdtemp(trackTitle);
    auto searchedTd = projectLocalDB->GetBuildedProject() << tdtemp;
    if(!searchedTd.has_value()) return;
    if(searchedTd->empty()) return;
    player.emplace(
        projectLocalDB->GetBuildedProject(),
        searchedTd->front(),
        frameBufferSize,
        true
    );
}

std::string 
editorObject::DESTROY_PROJECT()
{
    try
    {
        E_obj.reset();
        projectLocalDB.reset();
        auto deletedAmount = fs::remove_all(projectRoot);
        if(deletedAmount < 1){
            return "DELETED NOTHING";
        }
        else{
            return "COMPLETE";
        }
    }
    catch(const std::exception& e)
    {
        return e.what();
    }
}

#include <iostream>
bool
editorObject::ConfigNewMusic(
    const std::u8string& NewMusicName, 
    const std::u8string& composer,
    const fs::path& musicPath,
    const std::u8string& firstBar)
{
    if( E_obj->AddMusicConfig(NewMusicName)){
        std::cout << "DEBUGLINE: editorObject.cpp:96   " << TO_STR(NewMusicName) << "    " << TO_STR(composer) << std::endl;
        E_obj->musicHandle.back().jsonh["TITLE"] = TO_STR(NewMusicName);
        E_obj->musicHandle.back().jsonh["COMPOSER"] = TO_STR(composer);
        try
        {
            auto u8str =
            fs::relative(
                musicPath,
                projectRoot
            ).generic_u8string();
            auto safeStr = TO_STR(u8str);
            E_obj->musicHandle.back().jsonh["PATH"] = safeStr;
            std::cout << "DEBUGLINE: editorObject.cpp:108   " << safeStr << std::endl;
            std::cout << "DEBUGLINE: editorObject.cpp:109   " << musicPath << std::endl;
            std::cout << "DEBUGLINE: editorObject.cpp:110   " << projectRoot << std::endl;
        }
        catch(const std::exception& e)
        {
            RECENT_ERR = e.what();
            return false;
        }
        E_obj->musicHandle.back().jsonh["FIRST_BAR"] = TO_STR(firstBar);
        std::cout << "DEBUGLINE: editorObject.cpp:96   " << TO_STR(firstBar) << std::endl;
        return true;
    }
    else{
        return false;
    }
}

bool 
editorObject::Open(const fs::path& projectPath)
{
    projectRoot = projectPath;
    mixFilePath = projectPath / "Mixes" / "mixmetadata.PDJE";
    noteFilePath = projectPath / "Notes" / "notemetadata.PDJE";
    kvFilePath = projectPath / "KeyValues" / "keyvaluemetadata.PDJE";
    musicFileRootPath = projectPath / "Musics";
    projectLocalDB.emplace();
    
    return E_obj->openProject(projectPath) && projectLocalDB->Open(projectPath);
}

bool
editorObject::pushToRootDB(litedb& ROOTDB, const std::u8string& trackTitleToPush)
{
    TITLE_COMPOSER tcData;
    auto td = makeTrackData(trackTitleToPush, tcData);
    if(!(ROOTDB <= td)){
        return false;
    }
    for(auto& tcTemp : tcData){
        if(!pushToRootDB(ROOTDB, tcTemp.first, tcTemp.second)) continue;
    }
    return true;
}

#include <iostream>
bool 
editorObject::pushToRootDB(
    litedb& ROOTDB, 
    const std::u8string& musicTitle, 
    const std::u8string& musicComposer)
{
    std::cout << "DEBUGLINE: editorObject.cpp:160 " << TO_STR(musicTitle) << "  " << TO_STR(musicComposer) << std::endl;
    auto fromProjectSearchQuery = musdata(musicTitle, musicComposer);
    auto searched =
    projectLocalDB->GetBuildedProject() << fromProjectSearchQuery;
    if(!searched.has_value()){
        std::cout << "DEBUGLINE: editorObject.cpp:165 searched has no value" << std::endl;
        return false;
    }
    if(searched->empty()){
        std::cout << "DEBUGLINE: editorObject.cpp:169 searched empty" << std::endl;
        return false;
    }
    auto checkRoot = ROOTDB << searched->front();
    if(checkRoot.has_value()){
        if(!checkRoot->empty()){
            std::cout << "DEBUGLINE: editorObject.cpp:175 checkRoot not empty" << std::endl;
            return false;
        }
    }
    else{
        std::cout << "DEBUGLINE: editorObject.cpp:180 checkRoot has no value" << std::endl;
        return false;

    }
    auto resultToInsert = searched->front();
    std::cout << "Debugline: editorobj 185  " <<TO_STR(resultToInsert.musicPath) << std::endl;
    try{
        resultToInsert.musicPath =
        fs::relative(
            (projectRoot / fs::path(resultToInsert.musicPath)).lexically_normal(),
            ROOTDB.getRoot().parent_path()
        ).generic_u8string();
    }
    catch(std::exception e){
        RECENT_ERR = e.what();
        std::cout << "DEBUGLINE: editorObject.cpp:194 ERR" << e.what() << std::endl;
        return false;
    }
    if(!(ROOTDB <= resultToInsert)){
        std::cout << "DEBUGLINE: editorObject.cpp:197 ROOTDB error" << std::endl;
        return false;
    }

    return true;
}