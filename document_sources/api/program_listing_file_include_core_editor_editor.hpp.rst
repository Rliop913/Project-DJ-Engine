
.. _program_listing_file_include_core_editor_editor.hpp:

Program Listing for File editor.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_editor.hpp>` (``include\core\editor\editor.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #include "EditorArgs.hpp"
   #include "MusicControlPanel.hpp"
   #include "MusicJsonHelper.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "TimeLine.hpp"
   #include "dbRoot.hpp"
   
   #include <exception>
   #include <filesystem>
   #include <memory>
   #include <system_error>
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "fileNameSanitizer.hpp"
   
   static bool
   MKDirs(const fs::path &path)
   {
       std::error_code ec;
       try {
   
           if (!fs::create_directories(path, ec)) {
               if (ec) {
   
                   critlog("failed to create music directories. Path: ");
                   critlog(path.generic_string());
                   return false;
               } else {
                   return true;
               }
           } else {
               return true;
           }
       } catch (const std::exception &e) {
           critlog("failed to create directories. What: ");
           critlog(e.what());
           return false;
       }
   }
   
   class PDJE_API PDJE_Editor {
     private:
       fs::path    pt;
       fs::path    music_root;
       std::string name;
       std::string email;
       // fs::path      mixp;
       // fs::path      notep;
       // fs::path      musicp;
       // fs::path      kvp;
   
     public:
       std::unique_ptr<PDJE_TIMELINE::TimeLine<MIX_W>> mixHandle;
       std::unique_ptr<PDJE_TIMELINE::TimeLine<NOTE_W>> noteHandle;
       std::unique_ptr<PDJE_TIMELINE::TimeLine<KV_W>> KVHandle;
       struct MusicHandleStruct {
   
           std::unique_ptr<PDJE_TIMELINE::TimeLine<MUSIC_W>> handle;
           MusicHandleStruct()                          = delete;
           MusicHandleStruct(const MusicHandleStruct &) = delete;
   
           MusicHandleStruct(MusicHandleStruct &&) noexcept = default;
           MusicHandleStruct &
           operator=(MusicHandleStruct &&) noexcept = default;
   
           MusicHandleStruct(const fs::path      &root,
                             const DONT_SANITIZE &auth_name,
                             const DONT_SANITIZE &auth_email)
           {
               handle = std::make_unique<PDJE_TIMELINE::TimeLine<MUSIC_W>>(
                   root, "musicmetadata.PDJE", auth_name, auth_email);
           }
   
           MusicHandleStruct(const fs::path      &root,
                             const SANITIZED     &musicTitle,
                             const SANITIZED     &composer,
                             const DONT_SANITIZE &firstBeat,
                             const fs::path      &location,
                             const DONT_SANITIZE &auth_name,
                             const DONT_SANITIZE &auth_email)
           {
               if (!MKDirs(root)) {
                   return;
               }
               handle = std::make_unique<PDJE_TIMELINE::TimeLine<MUSIC_W>>(
                   root, "musicmetadata.PDJE", auth_name, auth_email);
               ConfigMusicJsonData(
                   *handle->GetJson(), musicTitle, composer, firstBeat, location);
           }
       };
       std::vector<MusicHandleStruct> musicHandle;
   
       bool
       AddMusicConfig(const SANITIZED     &NewMusicName,
                      const SANITIZED     &Composer,
                      const DONT_SANITIZE &firstBeat,
                      const fs::path      &music_location);
   
       // void
       // openProject();
   
       PDJE_Editor(const fs::path      &root,
                   const DONT_SANITIZE &auth_name,
                   const DONT_SANITIZE &auth_email)
       {
           pt         = root;
           auto mixr  = root / "Mixes";
           auto noter = root / "Notes";
           auto kvr   = root / "KeyValues";
           music_root = root / "Musics";
           if (MKDirs(mixr) && MKDirs(noter) && MKDirs(kvr) &&
               MKDirs(music_root)) {
               mixHandle = std::make_unique<PDJE_TIMELINE::TimeLine<MIX_W>>(
                   mixr, "mixmetadata.PDJE", auth_name, auth_email);
               noteHandle = std::make_unique<PDJE_TIMELINE::TimeLine<NOTE_W>>(
                   noter, "notemetadata.PDJE", auth_name, auth_email);
               KVHandle = std::make_unique<PDJE_TIMELINE::TimeLine<KV_W>>(
                   kvr, "keyvaluemetadata.PDJE", auth_name, auth_email);
               for (const auto &musicSubpath :
                    fs::directory_iterator(music_root)) {
                   musicHandle.emplace_back(
                       musicSubpath.path(), auth_name, auth_email);
               }
           }
           name  = auth_name;
           email = auth_email;
       }
       ~PDJE_Editor() = default;
   };
