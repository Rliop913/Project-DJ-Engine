#include "editorObject.hpp"



trackdata
editorObject::makeTrackData(
    const std::string& trackTitle, 
    std::unordered_map<std::string, std::string>& titles)
{
    trackdata td;
    auto mixRendered = E_obj->mixHandle.second.render();
    auto mixData = mixRendered->Wp->getDatas();
    
    for(unsigned long long i=0; i < mixData.size();++i){
        if(mixData[i].getType() == TypeEnum::LOAD){
            auto first = std::string(mixData[i].getFirst().cStr());
            auto second = std::string(mixData[i].getSecond().cStr());
            // std::u8string first, second;
            // first.resize(mixData[i].getFirst().size());
            // second.resize(mixData[i].getSecond().size());
            // std::transform(
            //     mixData[i].getFirst().begin(), mixData[i].getFirst().end(),
            //     first.begin(),
            //     [](char c){return static_cast<char8_t>(c);}
            // );
            // std::transform(
            //     mixData[i].getSecond().begin(), mixData[i].getSecond().end(),
            //     second.begin(),
            //     [](char c){return static_cast<char8_t>(c);}
            // );
        
            titles.insert(std::pair(first, second));
        }
    }
    
    td.trackTitle = trackTitle;
    td.mixBinary = mixRendered->out();
    td.noteBinary = E_obj->noteHandle.second.render()->out();
    for(auto& i : titles){
        td.cachedMixList+= (i.first + ",");
    }
    if(!titles.empty()){
        td.cachedMixList.pop_back();
    }
    return std::move(td);
}

void 
editorObject::demoPlayInit(
    std::optional<audioPlayer>& player, 
    unsigned int frameBufferSize, const std::string& trackTitle)
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
    const std::string& NewMusicName, 
    const std::string& composer,
    const fs::path& musicPath,
    const std::string& firstBar)
{
    if( E_obj->AddMusicConfig(NewMusicName)){
        std::cout << "DEBUGLINE: editorObject.cpp:96   " << NewMusicName << "    " << composer << std::endl;
        E_obj->musicHandle.back().jsonh["TITLE"] = NewMusicName;
        E_obj->musicHandle.back().jsonh["COMPOSER"] = composer;
        try
        {
            if(!fs::exists(musicPath)){
                return false;
            }
            // auto safeStr =
            // fs::relative(
            //     musicPath,
            //     projectRoot
            // ).generic_string();
            fs::path absPath;
            if(musicPath.is_absolute()){
                absPath = musicPath;
            }
            else{
                absPath = fs::absolute(musicPath);
            }
            E_obj->musicHandle.back().jsonh["PATH"] = absPath;
            std::cout << "DEBUGLINE: editorObject.cpp:108   " << absPath << std::endl;
            std::cout << "DEBUGLINE: editorObject.cpp:109   " << musicPath << std::endl;
            std::cout << "DEBUGLINE: editorObject.cpp:110   " << projectRoot << std::endl;
        }
        catch(const std::exception& e)
        {
            RECENT_ERR = e.what();
            return false;
        }
        E_obj->musicHandle.back().jsonh["FIRST_BAR"] = firstBar;
        std::cout << "DEBUGLINE: editorObject.cpp:96   " << firstBar << std::endl;
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
editorObject::pushToRootDB(litedb& ROOTDB, const std::string& trackTitleToPush)
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
    const std::string& musicTitle, 
    const std::string& musicComposer)
{
    std::cout << "DEBUGLINE: editorObject.cpp:160 " << musicTitle << "  " << musicComposer << std::endl;
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
    std::cout << "Debugline: editorobj 185  " <<(resultToInsert.musicPath) << std::endl;
    try{
        resultToInsert.musicPath = PDJE_Name_Sanitizer::getFileName(musicTitle + musicComposer);
        ROOTDB.KVPut(k,v)//no impl crash
        
        // auto musicRelPath = (projectRoot / fs::path(resultToInsert.musicPath)).lexically_normal();
        // auto RootRelPath = ROOTDB.getRoot().parent_path().lexically_normal();
        // RootRelPath = RootRelPath.string().empty() ? fs::path("."): RootRelPath;
        // resultToInsert.musicPath =
        // fs::relative(
        //     musicRelPath,
        //     RootRelPath
        // ).generic_string();
    }
    catch(std::exception e){
        RECENT_ERR = e.what();
        std::cout << "DEBUGLINE: editorObject.cpp:194 ERR" << e.what() << std::endl;
        return false;
    }
    std::cout << "Debugline: editorobj 198--------------  " <<(resultToInsert.musicPath) << std::endl;
    if(!(ROOTDB <= resultToInsert)){
        std::cout << "DEBUGLINE: editorObject.cpp:197 ROOTDB error" << std::endl;
        return false;
    }

    return true;
}