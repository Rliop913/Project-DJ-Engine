
.. _program_listing_file_core_include_MainObjects_editorObject_editorObject.cpp:

Program Listing for File editorObject.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_MainObjects_editorObject_editorObject.cpp>` (``core_include\MainObjects\editorObject\editorObject.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "fileNameSanitizer.hpp"
   #include "tempDB.hpp"
   #include "trackDB.hpp"
   
   trackdata
   editorObject::makeTrackData(const UNSANITIZED &trackTitle,
                               std::unordered_map<SANITIZED, SANITIZED> &titles)
   {
       trackdata td;
       auto      mixRendered = E_obj->mixHandle.second.render();
       auto      mixData     = mixRendered->Wp->getDatas();
   
       for (unsigned long long i = 0; i < mixData.size(); ++i) {
           if (mixData[i].getType() == TypeEnum::LOAD) {
               auto first  = SANITIZED(mixData[i].getFirst().cStr());
               auto second = SANITIZED(mixData[i].getSecond().cStr());
   
               titles.insert(std::pair(first, second));
           }
       }
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(trackTitle);
       if (!safeTitle) {
           critlog("failed to sanitize title. from editorObject makeTrackData. "
                   "trackTitle: ");
           critlog(trackTitle);
       }
       td.trackTitle = safeTitle.value();
       td.mixBinary  = mixRendered->out();
       td.noteBinary = E_obj->noteHandle.second.render()->out();
       for (auto &i : titles) {
           td.cachedMixList += (i.first + ",");
       }
       if (!titles.empty()) {
           td.cachedMixList.pop_back();
       }
       return std::move(td);
   }
   
   void
   editorObject::demoPlayInit(std::shared_ptr<audioPlayer> &player,
                              unsigned int                  frameBufferSize,
                              const UNSANITIZED            &trackTitle)
   {
       if (player) {
           player.reset();
       }
       trackdata tdtemp(trackTitle);
       auto      searchedTd = projectLocalDB->GetBuildedProject() << tdtemp;
       if (!searchedTd.has_value()) {
           critlog("failed to search trackdata from project local database. from "
                   "editorObject demoPlayInit. trackTitle: ");
           critlog(trackTitle);
           return;
       }
       if (searchedTd->empty()) {
           warnlog("cannot find trackdata from project local database. from "
                   "editorObject demoPlayInit. trackTitle: ");
           warnlog(trackTitle);
           return;
       }
       player = std::make_shared<audioPlayer>(projectLocalDB->GetBuildedProject(),
                                              searchedTd->front(),
                                              frameBufferSize,
                                              true);
   }
   
   DONT_SANITIZE
   editorObject::DESTROY_PROJECT()
   {
       try {
           E_obj.reset();
           projectLocalDB.reset();
           auto deletedAmount = fs::remove_all(projectRoot);
           if (deletedAmount < 1) {
               return "DELETED NOTHING";
           } else {
               return "COMPLETE";
           }
       } catch (const std::exception &e) {
           critlog("failed to destroy project. from editorObject DESTROY_PROJECT "
                   "ErrException: ");
           critlog(e.what());
           return e.what();
       }
   }
   
   bool
   editorObject::ConfigNewMusic(const UNSANITIZED   &NewMusicName,
                                const UNSANITIZED   &composer,
                                const fs::path      &musicPath,
                                const DONT_SANITIZE &firstBeat)
   {
       auto safeMus      = PDJE_Name_Sanitizer::sanitizeFileName(NewMusicName);
       auto safeComposer = PDJE_Name_Sanitizer::sanitizeFileName(composer);
       if (!safeMus.has_value() || !safeComposer.has_value()) {
           critlog("failed to sanitize in editorObject ConfigNewMusic. datas: ");
           critlog(NewMusicName);
           critlog(composer);
           return false;
       }
       if (!fs::exists(musicPath)) {
           critlog("music path does not exists. from editorObject "
                   "ConfigNewMusic. path: ");
           critlog(musicPath.generic_string());
           return false;
       }
       ma_decoder        test_decoder;
       ma_decoder_config test_decconf =
           ma_decoder_config_init(ma_format_s32, 2, 48000);
   
       auto init_result = ma_decoder_init_file(
           musicPath.string().c_str(), &test_decconf, &test_decoder);
       ma_decoder_uninit(&test_decoder);
       if (init_result != MA_SUCCESS) {
           critlog("music file is not available. from editorObject "
                   "ConfigNewMusic. path:");
           critlog(musicPath.generic_string());
           return false;
       }
       fs::path tempDataPath;
       if (E_obj->AddMusicConfig(safeMus.value(), tempDataPath)) {
   
           E_obj->musicHandle.back().jsonh[PDJE_JSON_TITLE] = safeMus.value();
           E_obj->musicHandle.back().jsonh[PDJE_JSON_COMPOSER] =
               safeComposer.value();
           E_obj->musicHandle.back().dataPath = tempDataPath;
           try {
               fs::path absPath;
               if (musicPath.is_absolute()) {
                   absPath = musicPath.lexically_normal();
               } else {
                   absPath = fs::absolute(musicPath).lexically_normal();
               }
               E_obj->musicHandle.back().jsonh[PDJE_JSON_PATH] = absPath;
           } catch (const std::exception &e) {
               critlog("something failed in editorObject ConfigNewMusic. "
                       "ErrException: ");
               critlog(e.what());
               return false;
           }
           E_obj->musicHandle.back().jsonh[PDJE_JSON_FIRST_BEAT] = firstBeat;
           return true;
       } else {
           critlog("failed to add music config. from editorObject ConfigNewMusic. "
                   "musicName: ");
           critlog(NewMusicName);
   
           return false;
       }
   }
   
   bool
   editorObject::Open(const fs::path &projectPath)
   {
       projectRoot       = projectPath;
       mixFilePath       = projectPath / "Mixes" / "mixmetadata.PDJE";
       noteFilePath      = projectPath / "Notes" / "notemetadata.PDJE";
       kvFilePath        = projectPath / "KeyValues" / "keyvaluemetadata.PDJE";
       musicFileRootPath = projectPath / "Musics";
       projectLocalDB.emplace();
   
       return E_obj->openProject(projectPath) && projectLocalDB->Open(projectPath);
   }
   
   bool
   editorObject::pushToRootDB(litedb &ROOTDB, const UNSANITIZED &trackTitleToPush)
   {
       trackdata searchQuery;
       searchQuery.trackTitle =
           PDJE_Name_Sanitizer::sanitizeFileName(trackTitleToPush).value_or("");
       auto localSearched = projectLocalDB->GetBuildedProject() << searchQuery;
       if (!localSearched.has_value()) {
           critlog("failed to search track data. from editorObject "
                   "pushToRootDB(litedb, UNSANITIZED); trackTitle: ");
           critlog(trackTitleToPush.c_str());
           return false;
       }
       if (localSearched->size() < 1) {
           warnlog("cannot find track data from data base. from editorObject "
                   "pushToRootDB(litedb, UNSANITIZED);");
           return false;
       }
   
       TITLE_COMPOSER tcData;
       auto           td = makeTrackData(trackTitleToPush, tcData);
       if (!(ROOTDB <= td)) {
           critlog("failed to push trackdata to root database. from editorObject "
                   "pushToRootDB. trackTitle: ");
           critlog(trackTitleToPush);
           return false;
       }
       for (auto &tcTemp : tcData) {
           UNSANITIZED musTitle = PDJE_Name_Sanitizer::getFileName(tcTemp.first);
           UNSANITIZED musComposer =
               PDJE_Name_Sanitizer::getFileName(tcTemp.second);
           pushToRootDB(ROOTDB, musTitle, musComposer);
       }
       return true;
   }
   
   bool
   editorObject::pushToRootDB(litedb            &ROOTDB,
                              const UNSANITIZED &musicTitle,
                              const UNSANITIZED &musicComposer)
   {
       auto fromProjectSearchQuery = musdata(musicTitle, musicComposer);
       auto searched               = projectLocalDB->GetBuildedProject()
                       << fromProjectSearchQuery;
       if (!searched.has_value()) {
           critlog("searched has no value. from editorObject pushToRootDB. "
                   "musicTitle & composer: ");
           critlog(musicTitle);
           critlog(musicComposer);
           return false;
       }
       if (searched->empty()) {
           warnlog("searched is empty. from editorObject pushToRootDB. musicTitle "
                   "& composer: ");
           warnlog(musicTitle);
           warnlog(musicComposer);
           return false;
       }
       auto checkRoot = ROOTDB << searched->front();
       if (checkRoot.has_value()) {
           if (!checkRoot->empty()) {
               warnlog("checkRoot not empty. from editorObject pushToRootDB. "
                       "musicTitle & composer: ");
               warnlog(musicTitle);
               warnlog(musicComposer);
               return false;
           }
       } else {
           critlog("checkRoot has no value. from editorObject pushToRootDB. "
                   "musicTitle & composer: ");
           critlog(musicTitle);
           critlog(musicComposer);
           return false;
       }
       auto resultToInsert = searched->front();
       try {
           auto Key =
               PDJE_Name_Sanitizer::sanitizeFileName(musicTitle + musicComposer);
           if (!Key) {
               critlog("failed to sanitize musicTitle + musicComposer. from "
                       "editorObject pushToRootDB. musicTitle & composer: ");
               critlog(musicTitle);
               critlog(musicComposer);
               return false;
           }
           resultToInsert.musicPath = Key.value();
   
           auto originMusicPath = fs::path(searched->front().musicPath);
           if (!fs::exists(originMusicPath)) {
               critlog("origin music path does not exists. from editorObject "
                       "pushToRootDB. path: ");
               critlog(originMusicPath.generic_string());
               return false;
           }
           std::ifstream        musicFile(originMusicPath, std::ios::binary);
           std::vector<uint8_t> fileData{ std::istreambuf_iterator<char>(
                                              musicFile),
                                          std::istreambuf_iterator<char>() };
           std::string MusBin(reinterpret_cast<const char *>(fileData.data()),
                              fileData.size());
           if (!ROOTDB.KVPut(resultToInsert.musicPath, MusBin)) {
               critlog(
                   "KVPUT failed. from editorObject pushToRootDB. musicPath: ");
               critlog(resultToInsert.musicPath);
               return false;
           }
   
       } catch (std::exception &e) {
           critlog(
               "something failed in editorObject pushToRootDB. ErrException: ");
           critlog(e.what());
           return false;
       }
       if (!(ROOTDB <= resultToInsert)) {
           critlog("failed to push musicdata to root database. from editorObject "
                   "pushToRootDB. musicTitle & composer: ");
           critlog(musicTitle);
           critlog(musicComposer);
           return false;
       }
   
       return true;
   }
