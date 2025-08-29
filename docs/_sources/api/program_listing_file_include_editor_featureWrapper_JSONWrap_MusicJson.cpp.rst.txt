
.. _program_listing_file_include_editor_featureWrapper_JSONWrap_MusicJson.cpp:

Program Listing for File MusicJson.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_JSONWrap_MusicJson.cpp>` (``include\editor\featureWrapper\JSONWrap\MusicJson.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "jsonWrapper.hpp"
   
   #include "editorObject.hpp"
   
   template <>
   template <>
   int
   PDJE_JSONHandler<MUSIC_W>::deleteLine(const NoteArgs &args,
                                         bool            skipType,
                                         bool            skipDetail) = delete;
   
   template <>
   template <>
   int
   PDJE_JSONHandler<MUSIC_W>::deleteLine(const MusicArgs &args)
   {
       std::vector<int> targetIDX;
       try {
           for (unsigned long long i = 0; i < ROOT[PDJEMUSICBPM].size(); ++i) {
               auto Target = ROOT[PDJEMUSICBPM].at(i);
               if (Target[PDJE_JSON_BEAT] != args.beat && args.beat != -1)
                   continue;
               if (Target[PDJE_JSON_SUBBEAT] != args.subBeat && args.subBeat != -1)
                   continue;
               if (Target[PDJE_JSON_SEPARATE] != args.separate && args.separate != -1)
                   continue;
               if (Target[PDJE_JSON_BPM] != args.bpm && args.bpm != "")
                   continue;
   
               targetIDX.push_back(i);
           }
           for (auto i : targetIDX | vs::reverse) {
               ROOT[PDJEMUSICBPM].erase(i);
           }
       } catch (...) {
           return 0;
       }
       return static_cast<int>(targetIDX.size());
   }
   
   template <>
   template <>
   bool
   PDJE_JSONHandler<MUSIC_W>::add(const MusicArgs &args)
   {
       nj tempMus = { { PDJE_JSON_BPM, args.bpm },
                      { PDJE_JSON_BEAT, args.beat },
                      { PDJE_JSON_SUBBEAT, args.subBeat },
                      { PDJE_JSON_SEPARATE, args.separate } };
       if (!ROOT.contains(PDJEMUSICBPM)) {
           critlog(
               "music json root not found. from PDJE_JSONHandler<MUSIC_W> add.");
           return false;
       }
       ROOT[PDJEMUSICBPM].push_back(tempMus);
       return true;
   }
   
   template <>
   std::unique_ptr<MUSIC_W>
   PDJE_JSONHandler<MUSIC_W>::render()
   {
       try {
           auto tempMusBin = std::make_unique<MUSIC_W>();
           tempMusBin->makeNew();
           auto rootsz = ROOT[PDJEMUSICBPM].size();
           auto filler = tempMusBin->Wp->initDatas(rootsz);
           for (std::size_t i = 0; i < rootsz; ++i) {
               auto target = ROOT[PDJEMUSICBPM].at(i);
               filler[i].setBeat(target[PDJE_JSON_BEAT]);
               filler[i].setSubBeat(target[PDJE_JSON_SUBBEAT]);
               filler[i].setBpm(target[PDJE_JSON_BPM].get<DONT_SANITIZE>());
               filler[i].setSeparate(target[PDJE_JSON_SEPARATE]);
           }
           return tempMusBin;
       } catch (std::exception &e) {
           critlog("something failed. from PDJE_JSONHandler<MUSIC_W> render. "
                   "ErrException: ");
           critlog(e.what());
           return nullptr;
       }
   }
   
   template <>
   template <>
   void
   PDJE_JSONHandler<MUSIC_W>::getAll(
       std::function<void(const EDIT_ARG_MUSIC &args)> jsonCallback)
   {
       if (!ROOT.contains(PDJEMUSICBPM)) {
           critlog("music json root not found. from PDJE_JSONHandler<MUSIC_W> "
                   "getAll.");
           return;
       }
       for (auto &i : ROOT[PDJEMUSICBPM]) {
           EDIT_ARG_MUSIC tempargs;
   
           tempargs.musicName =
               PDJE_Name_Sanitizer::getFileName(ROOT[PDJE_JSON_TITLE].get<SANITIZED>());
   
           auto tempBpm = i[PDJE_JSON_BPM].get<DONT_SANITIZE>();
           tempargs.arg = { tempBpm, i[PDJE_JSON_BEAT], i[PDJE_JSON_SUBBEAT], i[PDJE_JSON_SEPARATE] };
           jsonCallback(tempargs);
       }
   }
   template <>
   bool
   PDJE_JSONHandler<MUSIC_W>::load(const fs::path &path)
   {
       auto filepath = path / "musicmetadata.PDJE";
       if (fs::exists(filepath)) {
           if (fs::is_regular_file(filepath)) {
               std::ifstream jfile(filepath);
   
               if (!jfile.is_open()) {
                   critlog("cannot open music json file. from "
                           "PDJE_JSONHandler<MUSIC_W> load. path: ");
                   critlog(path.generic_string());
                   return false;
               }
   
               try {
                   jfile >> ROOT;
               } catch (std::exception &e) {
                   critlog("cannot load music json data from file. from "
                           "PDJE_JSONHandler<MUSIC_W> load. ErrException: ");
                   critlog(e.what());
                   return false;
               }
   
               jfile.close();
           } else {
               critlog("music json file path is not regular file.  from "
                       "PDJE_JSONHandler<MUSIC_W> load. path: ");
               critlog(path.generic_string());
               return false;
           }
       } else {
           fs::create_directories(filepath.parent_path());
           std::ofstream jfile(filepath);
           if (!jfile.is_open()) {
               critlog("cannot open or make new music json file. from "
                       "PDJE_JSONHandler<MUSIC_W> load. path: ");
               critlog(path.generic_string());
               return false;
           }
           jfile << std::setw(4) << ROOT;
           jfile.close();
       }
   
       if (!ROOT.contains(PDJEMUSICBPM)) {
           ROOT[PDJEMUSICBPM] = nj::array();
       }
   
       return true;
   }
