
.. _program_listing_file_include_MainObjects_editorObject_render.cpp:

Program Listing for File render.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_render.cpp>` (``include/MainObjects/editorObject/render.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   bool
   editorObject::render(const UNSANITIZED &trackTitle, litedb &ROOTDB)
   {
       std::unordered_map<SANITIZED, SANITIZED> titles;
       auto td = makeTrackData(trackTitle, titles);
   
       std::vector<musdata> mds;
       for (auto &i : E_obj->musicHandle) {
           if(i.musicName == "" || !fs::exists(i.dataPath)){
               continue;
           }
           mds.emplace_back();
   
           auto rendered    = i.jsonh.render();
           mds.back().title = i.musicName;
           auto rdout       = rendered->out();
           mds.back().bpmBinary.assign(rdout.begin(), rdout.end());
           auto tempCOMPOSER   = i.jsonh[PDJE_JSON_COMPOSER].get<SANITIZED>();
           auto tempPATH       = i.jsonh[PDJE_JSON_PATH].get<SANITIZED>();
           auto tempFIRST_BEAT = i.jsonh[PDJE_JSON_FIRST_BEAT].get<DONT_SANITIZE>();
   
           mds.back().composer  = (tempCOMPOSER);
           mds.back().musicPath = (tempPATH);
           mds.back().firstBeat = (tempFIRST_BEAT);
           try {
               mds.back().bpm =
                   std::stod(rendered->Wp->getDatas()[0].getBpm().cStr());
           } catch (std::exception &e) {
               critlog("failed to convert bpm to double. from editorObject "
                       "render. ErrException: ");
               critlog(e.what());
               critlog("music name: ");
               critlog(i.musicName);
               continue;
           }
           titles[i.musicName] = "";
       }
   
       for (auto &i : titles) {
           if (i.second != "") {
               auto findFromRoot     = musdata();
               findFromRoot.title    = i.first;
               findFromRoot.composer = i.second;
               auto mus              = ROOTDB << findFromRoot;
               if (mus.has_value()) {
                   if (mus->empty())
                       continue;
                   musdata fromRoot = mus->front();
                   try {
                       fromRoot.musicPath =
                           fs::relative(
                               fs::absolute(ROOTDB.getRoot().parent_path() /
                                            fromRoot.musicPath),
                               projectRoot)
                               .string();
                   } catch (std::exception &e) {
                       critlog("failed to convert relative path. from "
                               "editorObject render. ErrException: ");
                       critlog(e.what());
                       critlog("music path: ");
                       critlog(fromRoot.musicPath);
                       critlog("project root: ");
                       critlog(projectRoot.generic_string());
                       return false;
                   }
                   mds.push_back(fromRoot);
               }
           }
       }
   
       return projectLocalDB->BuildProject(td, mds);
   }
