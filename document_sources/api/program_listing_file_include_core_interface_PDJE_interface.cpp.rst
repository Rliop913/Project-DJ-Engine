
.. _program_listing_file_include_core_interface_PDJE_interface.cpp:

Program Listing for File PDJE_interface.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_interface_PDJE_interface.cpp>` (``include\core\interface\PDJE_interface.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_interface.hpp"
   #include "Decoder.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "musicDB.hpp"
   
   #include <optional>
   #include <stdexcept>
   
   namespace {
   
   std::optional<SANITIZED_ORNOT>
   ResolveMusicKeyOrPath(litedb &db, const musdata &md)
   {
       if (!md.musicPath.empty()) {
           return md.musicPath;
       }
   
       const bool has_search_filter =
           !md.title.empty() || !md.composer.empty() || md.bpm >= 0;
       if (!has_search_filter) {
           warnlog("cannot resolve music path from musdata without search clues. "
                   "from PDJE GetPCMFromMusData");
           return std::nullopt;
       }
   
       musdata query     = md;
       auto    searchRes = db << query;
       if (!searchRes.has_value()) {
           critlog("failed to search music from database. from PDJE "
                   "GetPCMFromMusData");
           return std::nullopt;
       }
       if (searchRes->empty()) {
           warnlog("cannot find music from database. from PDJE GetPCMFromMusData");
           return std::nullopt;
       }
       if (searchRes->front().musicPath.empty()) {
           warnlog("resolved music entry has empty musicPath. from PDJE "
                   "GetPCMFromMusData");
           return std::nullopt;
       }
       return searchRes->front().musicPath;
   }
   
   } // namespace
   
   PDJE::PDJE(const DONT_SANITIZE &rootPath)
   {
       DBROOT = std::make_shared<litedb>();
       DBROOT->openDB(rootPath);
   }
   
   TRACK_VEC
   PDJE::SearchTrack(const UNSANITIZED &Title)
   {
       trackdata td;
       auto      safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(Title);
       if (!safeTitle) {
           critlog("failed to make sanitized filename in PDJE SearchTrack. "
                   "Errtitle: ");
           critlog(Title);
           return TRACK_VEC();
       }
       td.trackTitle = safeTitle.value();
       auto dbres    = (*DBROOT) << td;
       if (dbres.has_value()) {
           return dbres.value();
       } else {
           warnlog("failed to find trackdata from PDJE database. Errtitle: ");
           warnlog(Title);
           return TRACK_VEC();
       }
   }
   
   MUS_VEC
   PDJE::SearchMusic(const UNSANITIZED &Title,
                     const UNSANITIZED &composer,
                     const double       bpm)
   {
       musdata md;
       auto    safeTitle    = PDJE_Name_Sanitizer::sanitizeFileName(Title);
       auto    safeComposer = PDJE_Name_Sanitizer::sanitizeFileName(composer);
       if (!safeTitle || !safeComposer) {
           critlog("failed to sanitize filename in PDJE SearchMusic. Errs: ");
           critlog(Title);
           critlog(composer);
           return MUS_VEC();
       }
       md.title    = safeTitle.value();
       md.composer = safeComposer.value();
       md.bpm      = bpm;
       auto dbres  = (*DBROOT) << md;
       if (dbres.has_value()) {
           return dbres.value();
       } else {
           warnlog("failed to find music from PDJE database. ErrTitle: ");
           warnlog(Title);
           warnlog(composer);
           return MUS_VEC();
       }
   }
   
   std::vector<float>
   PDJE::GetPCMFromMusData(const musdata &md, const int chSZ)
   {
       if (!DBROOT) {
           critlog(
               "database root is not initialized. from PDJE GetPCMFromMusData");
           return {};
       }
   
       auto keyOrPath = ResolveMusicKeyOrPath(*DBROOT, md);
       if (!keyOrPath.has_value()) {
           return {};
       }
   
       Decoder decoder;
       if (!decoder.init(*DBROOT, keyOrPath.value(), chSZ)) {
           critlog("failed to initialize decoder. from PDJE GetPCMFromMusData");
           return {};
       }
   
       ma_uint64 frameCount = 0;
       if (ma_decoder_get_available_frames(&decoder.dec, &frameCount) !=
           MA_SUCCESS) {
           critlog("failed to get available pcm frames. from PDJE "
                   "GetPCMFromMusData");
           return {};
       }
   
       std::vector<float> pcm;
       if (!decoder.getRange(static_cast<FRAME_POS>(frameCount), pcm, chSZ)) {
           critlog("failed to decode pcm frames. from PDJE GetPCMFromMusData");
           return {};
       }
   
       return pcm;
   }
   
   bool
   PDJE::InitPlayer(PLAY_MODE          mode,
                    trackdata         &td,
                    const unsigned int FrameBufferSize)
   {
       try {
           switch (mode) {
           case PLAY_MODE::FULL_PRE_RENDER:
               player = std::make_shared<audioPlayer>(
                   (*DBROOT), td, FrameBufferSize, false);
               break;
           case PLAY_MODE::HYBRID_RENDER:
               player = std::make_shared<audioPlayer>(
                   (*DBROOT), td, FrameBufferSize, true);
               break;
           case PLAY_MODE::FULL_MANUAL_RENDER:
               player = std::make_shared<audioPlayer>(FrameBufferSize);
               break;
   
           default:
               break;
           }
       } catch (const std::exception &e) {
           critlog("failed to init player on PDJE initPlayer. why: ");
           critlog(e.what());
           return false;
       }
   
       if (!player) {
           critlog("failed to init player on PDJE initPlayer.");
           return false;
       } else {
           return true;
       }
   }
   
   bool
   PDJE::GetNoteObjects(trackdata &td, OBJ_SETTER_CALLBACK &ObjectSetCallback)
   {
       CapReader<NoteBinaryCapnpData> notereader;
       CapReader<MixBinaryCapnpData>  mixreader;
   
       if (!notereader.open(td.noteBinary)) {
           warnlog("failed to read notebinary from trackdata from PDJE "
                   "GetNoteObjects");
           return false;
       }
       if (!mixreader.open(td.mixBinary)) {
           warnlog(
               "failed to read mixBinary from trackdata from PDJE GetNoteObjects");
           return false;
       }
   
       auto noteTrans = new NoteTranslator();
       auto mixTrans  = new MixTranslator();
   
       mixTrans->Read(mixreader);
   
       if (mixTrans->bpms.has_value()) {
           noteTrans->Read(
               notereader, mixTrans->bpms.value().bpmVec, ObjectSetCallback);
       } else {
           critlog("failed to emplace optional object from PDJE GetNoteObjects");
           delete noteTrans;
           delete mixTrans;
           return false;
       }
       delete noteTrans;
       delete mixTrans;
       return true;
   }
   
   bool
   PDJE::InitEditor(const DONT_SANITIZE &auth_name,
                    const DONT_SANITIZE &auth_email,
                    const DONT_SANITIZE &projectRoot)
   {
       editor = std::make_shared<editorObject>();
       return editor->Open(projectRoot, auth_name, auth_email);
   }
   
   PDJE_CORE_DATA_LINE
   PDJE::PullOutDataLine()
   {
       if (player) {
           return player->PullOutDataLine();
       } else {
           PDJE_CORE_DATA_LINE errline;
           return errline;
       }
   }
   
   bool
   PDJE::DeleteMusic(musdata &target)
   {
   
       return DBROOT->DeleteData(target);
   }
   
   bool
   PDJE::DeleteTrack(trackdata &target)
   {
       return DBROOT->DeleteData(target);
   }
