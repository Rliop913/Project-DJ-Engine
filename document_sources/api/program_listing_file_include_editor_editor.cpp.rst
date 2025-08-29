
.. _program_listing_file_include_editor_editor.cpp:

Program Listing for File editor.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_editor.cpp>` (``include\editor\editor.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editor.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <filesystem>
   
   namespace fs = std::filesystem;
   
   bool
   PDJE_Editor::openProject(const fs::path &projectPath)
   {
       pt     = projectPath;
       mixp   = pt / "Mixes";
       notep  = pt / "Notes";
       musicp = pt / "Musics";
       kvp    = pt / "KeyValues";
   
       if (!fs::exists(pt) || !fs::is_directory(pt)) {
           fs::create_directory(pt);
           fs::create_directory(mixp);
           fs::create_directory(notep);
           fs::create_directory(musicp);
           fs::create_directory(kvp);
           if (!fs::exists(pt) || !fs::is_directory(pt) || !fs::exists(mixp) ||
               !fs::is_directory(mixp) || !fs::exists(notep) ||
               !fs::is_directory(notep) || !fs::exists(kvp) ||
               !fs::is_directory(kvp) || !fs::exists(musicp) ||
               !fs::is_directory(musicp)) {
               critlog("some path is not created. from PDJE_Editor openProject. "
                       "printing path.");
               critlog("editor project root: ");
               critlog(pt.generic_string());
               critlog("mix data directory: ");
               critlog(mixp.generic_string());
               critlog("note data directory: ");
               critlog(notep.generic_string());
               critlog("music data directory: ");
               critlog(musicp.generic_string());
               critlog("key value data directory: ");
               critlog(kvp.generic_string());
   
               return false;
           }
       }
       if (!mixHandle.first->Open(mixp) || !mixHandle.second.load(mixp) ||
           !KVHandler.first->Open(kvp) || !KVHandler.second.load(kvp) ||
           !noteHandle.first->Open(notep) || !noteHandle.second.load(notep)) {
           critlog("failed to open & load some project from PDJE_Editor "
                   "openProject. printing path");
           critlog("editor project root: ");
           critlog(pt.generic_string());
           critlog("mix data directory: ");
           critlog(mixp.generic_string());
           critlog("note data directory: ");
           critlog(notep.generic_string());
           critlog("music data directory: ");
           critlog(musicp.generic_string());
           critlog("key value data directory: ");
           critlog(kvp.generic_string());
           return false;
       }
   
       for (const auto &musicSubpath : fs::directory_iterator(musicp)) {
           if (fs::is_directory(musicSubpath)) {
   
               musicHandle.emplace_back(name, email);
               musicHandle.back().musicName =
                   musicSubpath.path().filename().string();
               if (!musicHandle.back().gith->Open(musicSubpath.path()) ||
                   !musicHandle.back().jsonh.load(musicSubpath.path())) {
                   critlog("failed to open & load some music configure project "
                           "from PDJE_Editor openProject. musicPath: ");
                   auto logPath = musicSubpath.path();
                   critlog(logPath.generic_string());
                   return false;
               }
           }
       }
       return true;
   }
   #include <random>
   bool
   PDJE_Editor::AddMusicConfig(const SANITIZED &NewMusicName, fs::path &DataPath)
   {
       std::random_device                          rd;
       std::mt19937                                gen(rd());
       std::uniform_int_distribution<unsigned int> randomFilename(
           std::numeric_limits<unsigned int>::min(),
           std::numeric_limits<unsigned int>::max());
       std::optional<DONT_SANITIZE> mfilename;
       for (int TRY_COUNT = 0; TRY_COUNT < 50; ++TRY_COUNT) {
           DONT_SANITIZE tempFilename = std::to_string(randomFilename(gen));
           if (!fs::exists(musicp / fs::path(tempFilename))) {
               mfilename = tempFilename;
               break;
           }
       }
       if (!mfilename.has_value()) {
           warnlog(
               "failed to make filename. this could be error or we have terrible "
               "luck. try again or fix here. from PDJE_Editor AddMusicConfig.");
           return false;
       }
       DataPath = musicp / fs::path(mfilename.value());
       try {
           if (fs::create_directory(DataPath)) {
               musicHandle.emplace_back(name, email);
               musicHandle.back().musicName = NewMusicName;
               if (!musicHandle.back().gith->Open(DataPath) ||
                   !musicHandle.back().jsonh.load(DataPath)) {
                   fs::remove_all(DataPath);
                   critlog("failed to init git or json. from PDJE_Editor "
                           "AddMusicConfig.");
                   return false;
               } else
                   return true;
           }
       } catch (const std::exception &e) {
           critlog("something wrong on configure music. from PDJE_Editor "
                   "AddMusicConfig. ErrException: ");
           critlog(e.what());
           return false;
       }
       critlog("failed. on configure music. from PDJE_Editor AddMusicConfig. "
               "please check logs");
       return false;
   }
