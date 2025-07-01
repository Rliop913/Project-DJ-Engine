#include "editorObject.hpp"



trackdata
editorObject::makeTrackData(
    const UNSANITIZED& trackTitle, 
    std::unordered_map<SANITIZED, SANITIZED>& titles)
{
    trackdata td;
    auto mixRendered = E_obj->mixHandle.second.render();
    auto mixData = mixRendered->Wp->getDatas();
    
    for(unsigned long long i=0; i < mixData.size();++i){
        if(mixData[i].getType() == TypeEnum::LOAD){
            auto first = SANITIZED(mixData[i].getFirst().cStr());
            auto second = SANITIZED(mixData[i].getSecond().cStr());
        
            titles.insert(std::pair(first, second));
        }
    }
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(trackTitle);
    if(!safeTitle){
        throw "FAILED TO SANITIZE TITLE";
    }
    td.trackTitle = safeTitle.value();
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
    unsigned int frameBufferSize, const UNSANITIZED& trackTitle)
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

DONT_SANITIZE
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
    const UNSANITIZED& NewMusicName, 
    const UNSANITIZED& composer,
    const fs::path& musicPath,
    const DONT_SANITIZE& firstBar)
{
    auto safeMus = PDJE_Name_Sanitizer::sanitizeFileName(NewMusicName);
    auto safeComposer = PDJE_Name_Sanitizer::sanitizeFileName(composer);
    // fs::path dPath;
    if(!safeMus.has_value() || !safeComposer.has_value()){
        PDJE_Name_Sanitizer::PDJE_SANITIZE_ERROR+= "Failed to sanitize in Config Music\n";
        return false;
    }
    fs::path tempDataPath;
    if( E_obj->AddMusicConfig(safeMus.value(), tempDataPath)){
        std::cout << "DEBUGLINE: editorObject.cpp:96   " << safeMus.value() << "    " << safeComposer.value() << std::endl;
        E_obj->musicHandle.back().jsonh["TITLE"] = safeMus.value();
        E_obj->musicHandle.back().jsonh["COMPOSER"] = safeComposer.value();
        E_obj->musicHandle.back().dataPath = tempDataPath;
        try
        {
            if(!fs::exists(musicPath)){
                return false;
            }
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
editorObject::pushToRootDB(litedb& ROOTDB, const UNSANITIZED& trackTitleToPush)
{
    TITLE_COMPOSER tcData;
    auto td = makeTrackData(trackTitleToPush, tcData);
    if(!(ROOTDB <= td)){
        return false;
    }
    for(auto& tcTemp : tcData){
        UNSANITIZED musTitle = PDJE_Name_Sanitizer::getFileName(tcTemp.first);
        UNSANITIZED musComposer = PDJE_Name_Sanitizer::getFileName(tcTemp.second);
        if(!pushToRootDB(ROOTDB, musTitle, musComposer)) continue;
    }
    return true;
}

#include <iostream>
bool 
editorObject::pushToRootDB(
    litedb& ROOTDB, 
    const UNSANITIZED& musicTitle, 
    const UNSANITIZED& musicComposer)
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
        auto Key = PDJE_Name_Sanitizer::sanitizeFileName(musicTitle + musicComposer);
        if(!Key){
            return false;
        }
        resultToInsert.musicPath = Key.value();
        
        auto originMusicPath = fs::path(searched->front().musicPath);
        if(!fs::exists(originMusicPath)){
            PDJE_Name_Sanitizer::PDJE_SANITIZE_ERROR += "push to db Failed. can't find/open filepath\n";
            return false;
        }
        std::ifstream musicFile(originMusicPath, std::ios::binary);
        std::vector<uint8_t> fileData {
            std::istreambuf_iterator<char>(musicFile),
            std::istreambuf_iterator<char>()
        };
        std::string MusBin(reinterpret_cast<const char*>(fileData.data()), fileData.size());
        ROOTDB.KVPut(resultToInsert.musicPath, MusBin);
        
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
