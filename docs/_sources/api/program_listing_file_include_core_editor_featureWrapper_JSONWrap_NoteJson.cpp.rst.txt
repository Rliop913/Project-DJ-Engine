
.. _program_listing_file_include_core_editor_featureWrapper_JSONWrap_NoteJson.cpp:

Program Listing for File NoteJson.cpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_featureWrapper_JSONWrap_NoteJson.cpp>` (``include/core/editor/featureWrapper/JSONWrap/NoteJson.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "jsonWrapper.hpp"
   
   template <>
   template <>
   bool
   PDJE_JSONHandler<NOTE_W>::add(const NoteArgs &args)
   {
       nj tempMix = {
           { PDJE_JSON_NOTE_TYPE,
             SANITIZED_ORNOT(args.Note_Type.begin(), args.Note_Type.end()) },
           { PDJE_JSON_NOTE_DETAIL,
             SANITIZED_ORNOT(args.Note_Detail.begin(), args.Note_Detail.end()) },
           { PDJE_JSON_FIRST,
             SANITIZED_ORNOT(args.first.begin(), args.first.end()) },
           { PDJE_JSON_SECOND,
             SANITIZED_ORNOT(args.second.begin(), args.second.end()) },
           { PDJE_JSON_THIRD,
             SANITIZED_ORNOT(args.third.begin(), args.third.end()) },
           { PDJE_JSON_BEAT, args.beat },
           { PDJE_JSON_SUBBEAT, args.subBeat },
           { PDJE_JSON_SEPARATE, args.separate },
           { PDJE_JSON_EBEAT, args.Ebeat },
           { PDJE_JSON_ESUBBEAT, args.EsubBeat },
           { PDJE_JSON_ESEPARATE, args.Eseparate }
       };
       if (!ROOT.contains(PDJENOTE)) {
           critlog("note json root not found. from PDJE_JSONHandler<NOTE_W> add.");
           return false;
       }
       ROOT[PDJENOTE].push_back(tempMix);
       return true;
   }
   
   template <>
   template <>
   int
   PDJE_JSONHandler<NOTE_W>::deleteLine(const NoteArgs &args)
   {
       std::vector<int> targetIDX;
       try {
           for (unsigned long long i = 0; i < ROOT[PDJENOTE].size(); ++i) {
               auto Target = ROOT[PDJENOTE].at(i);
               if (Target[PDJE_JSON_NOTE_TYPE] != args.Note_Type &&
                   args.Note_Type != "")
                   continue;
               if (Target[PDJE_JSON_NOTE_DETAIL] != args.Note_Detail &&
                   args.Note_Detail != "")
                   continue;
               if (Target[PDJE_JSON_FIRST] != args.first && args.first != "")
                   continue;
               if (Target[PDJE_JSON_SECOND] != args.second && args.second != "")
                   continue;
               if (Target[PDJE_JSON_THIRD] != args.third && args.third != "")
                   continue;
               if (Target[PDJE_JSON_BEAT] != args.beat && args.beat != -1)
                   continue;
               if (Target[PDJE_JSON_SUBBEAT] != args.subBeat && args.subBeat != -1)
                   continue;
               if (Target[PDJE_JSON_SEPARATE] != args.separate &&
                   args.separate != -1)
                   continue;
               if (Target[PDJE_JSON_EBEAT] != args.Ebeat && args.Ebeat != -1)
                   continue;
               if (Target[PDJE_JSON_ESUBBEAT] != args.EsubBeat &&
                   args.EsubBeat != -1)
                   continue;
               if (Target[PDJE_JSON_ESEPARATE] != args.Eseparate &&
                   args.Eseparate != -1)
                   continue;
               targetIDX.push_back(i);
           }
           for (auto i : targetIDX | vs::reverse) {
               ROOT[PDJENOTE].erase(i);
           }
       } catch (...) {
           return 0;
       }
       return static_cast<int>(targetIDX.size());
   }
   
   template <>
   template <>
   void
   PDJE_JSONHandler<NOTE_W>::getAll(
       std::function<void(const NoteArgs &args)> jsonCallback)
   {
       if (!ROOT.contains(PDJENOTE)) {
           critlog("note json root not found. from PDJE_JSONHandler<NOTE_W> add.");
           return;
       }
       for (auto &i : ROOT[PDJENOTE]) {
   
           NoteArgs tempargs{ i[PDJE_JSON_NOTE_TYPE].get<SANITIZED_ORNOT>(),
                              i[PDJE_JSON_NOTE_DETAIL].get<SANITIZED_ORNOT>(),
                              i[PDJE_JSON_FIRST].get<SANITIZED_ORNOT>(),
                              i[PDJE_JSON_SECOND].get<SANITIZED_ORNOT>(),
                              i[PDJE_JSON_THIRD].get<SANITIZED_ORNOT>(),
                              i[PDJE_JSON_BEAT],
                              i[PDJE_JSON_SUBBEAT],
                              i[PDJE_JSON_SEPARATE],
                              i[PDJE_JSON_EBEAT],
                              i[PDJE_JSON_ESUBBEAT],
                              i[PDJE_JSON_ESEPARATE] };
           jsonCallback(tempargs);
       }
   }
   
   template <>
   std::unique_ptr<NOTE_W>
   PDJE_JSONHandler<NOTE_W>::render()
   {
       try {
           auto tempMixBin = std::make_unique<NOTE_W>();
           tempMixBin->makeNew();
           auto rootsz = ROOT[PDJENOTE].size();
           auto filler = tempMixBin->Wp->initDatas(rootsz);
           for (std::size_t i = 0; i < rootsz; ++i) {
               auto target = ROOT[PDJENOTE].at(i);
               filler[i].setNoteType(
                   target[PDJE_JSON_NOTE_TYPE].get<SANITIZED_ORNOT>());
               filler[i].setNoteDetail(
                   target[PDJE_JSON_NOTE_DETAIL].get<SANITIZED_ORNOT>());
               filler[i].setFirst(target[PDJE_JSON_FIRST].get<SANITIZED_ORNOT>());
               filler[i].setSecond(
                   target[PDJE_JSON_SECOND].get<SANITIZED_ORNOT>());
               filler[i].setThird(target[PDJE_JSON_THIRD].get<SANITIZED_ORNOT>());
               filler[i].setBeat(target[PDJE_JSON_BEAT]);
               filler[i].setSubBeat(target[PDJE_JSON_SUBBEAT]);
               filler[i].setSeparate(target[PDJE_JSON_SEPARATE]);
               filler[i].setEbeat(target[PDJE_JSON_EBEAT]);
               filler[i].setEsubBeat(target[PDJE_JSON_ESUBBEAT]);
               filler[i].setESeparate(target[PDJE_JSON_ESEPARATE]);
           }
           return tempMixBin;
       } catch (std::exception &e) {
           critlog("something wrong. from PDJE_JSONHandler<NOTE_W> render. "
                   "ErrException: ");
           critlog(e.what());
           return nullptr;
       }
   }
   
   template <>
   bool
   PDJE_JSONHandler<NOTE_W>::load(const fs::path &path)
   {
       auto filepath = path / "notemetadata.PDJE";
       if (fs::exists(filepath)) {
           if (fs::is_regular_file(filepath)) {
               std::ifstream jfile(filepath);
   
               if (!jfile.is_open()) {
                   critlog("cannot open note json file. from "
                           "PDJE_JSONHandler<NOTE_W> load. path: ");
                   critlog(path.generic_string());
                   return false;
               }
   
               try {
                   jfile >> ROOT;
               } catch (std::exception &e) {
                   critlog("cannot load note data from json file. from "
                           "PDJE_JSONHandler<NOTE_W> load. ErrException: ");
                   critlog(e.what());
                   return false;
               }
   
               jfile.close();
           } else {
               critlog("filepath is not regular file. from "
                       "PDJE_JSONHandler<NOTE_W> load. path: ");
               critlog(path.generic_string());
               return false;
           }
       } else {
           fs::create_directories(filepath.parent_path());
           std::ofstream jfile(filepath);
           if (!jfile.is_open())
               return false;
           jfile << std::setw(4) << ROOT;
           jfile.close();
       }
   
       if (!ROOT.contains(PDJENOTE)) {
           ROOT[PDJENOTE] = nj::array();
       }
   
       return true;
   }
