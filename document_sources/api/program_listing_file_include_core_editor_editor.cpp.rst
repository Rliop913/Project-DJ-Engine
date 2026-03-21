
.. _program_listing_file_include_core_editor_editor.cpp:

Program Listing for File editor.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_editor.cpp>` (``include\core\editor\editor.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editor.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "fileNameSanitizer.hpp"
   #include <botan/hash.h>
   #include <botan/hex.h>
   #include <botan/secmem.h>
   #include <cstdint>
   #include <filesystem>
   namespace fs = std::filesystem;
   
   bool
   PDJE_Editor::AddMusicConfig(const SANITIZED     &NewMusicName,
                               const SANITIZED     &Composer,
                               const DONT_SANITIZE &firstBeat,
                               const fs::path      &music_location)
   {
   
       auto hash_engine = Botan::HashFunction::create("MD5");
       if (!hash_engine) {
           critlog("failed to init hash engine on configure music step.");
           return false;
       }
       std::string strsum = (NewMusicName + Composer);
       hash_engine->update(reinterpret_cast<const uint8_t *>(strsum.data()),
                           strsum.size());
       Botan::secure_vector<uint8_t> res = hash_engine->final();
   
       std::string hash = Botan::hex_encode(res);
   
       auto DataPath = music_root / fs::path(hash);
       if (fs::exists(DataPath)) {
           return false;
       }
       try {
           musicHandle.emplace_back(DataPath,
                                    NewMusicName,
                                    Composer,
                                    firstBeat,
                                    music_location,
                                    name,
                                    email);
   
           return true;
   
       } catch (const std::exception &e) {
           critlog("something wrong on configure music. from PDJE_Editor "
                   "AddMusicConfig. ErrException: ");
           critlog(e.what());
           return false;
       }
   }
