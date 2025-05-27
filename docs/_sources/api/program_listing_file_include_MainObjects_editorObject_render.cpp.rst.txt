
.. _program_listing_file_include_MainObjects_editorObject_render.cpp:

Program Listing for File render.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_editorObject_render.cpp>` (``include/MainObjects/editorObject/render.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorObject.hpp"
   
   bool
   editorObject::render(const std::string& trackTitle, litedb& ROOTDB)
   {
       // trackdata td;
       // auto mixRendered = E_obj->mixHandle.second.render();
       // auto mixData = mixRendered->Wp->getDatas();
       // std::unordered_map<std::string, std::string> titles;
       // for(unsigned long long i=0; i < mixData.size();++i){
       //     if(mixData[i].getType() == TypeEnum::LOAD){
       //         titles.insert(std::pair(mixData[i].getFirst().cStr(), mixData[i].getSecond().cStr()));
       //     }
       // }
       
       // td.trackTitle = trackTitle;
       // td.mixBinary = mixRendered->out();
       // td.noteBinary = E_obj->noteHandle.second.render()->out();
       // for(auto& i : titles){
       //     td.cachedMixList+= (i.first + ",");
       // }
       // if(!titles.empty()){
       //     td.cachedMixList.pop_back();
       // }
       std::unordered_map<std::string, std::string> titles;
       auto td = makeTrackData(trackTitle, titles);
       
       std::vector<musdata> mds;
       for(auto& i : E_obj->musicHandle){
           mds.emplace_back();
           
           auto rendered = i.jsonh.render();
           mds.back().title = i.musicName;
           mds.back().bpmBinary = rendered->out();
           mds.back().composer = i.jsonh["COMPOSER"];
           mds.back().musicPath = i.jsonh["PATH"];
           mds.back().firstBar = i.jsonh["FIRST_BAR"];
           try{
               mds.back().bpm = std::stod(rendered->Wp->getDatas()[0].getBpm().cStr());
           }
           catch(...){
               continue;
           }
           titles[i.musicName] = "";
       }
   
       for(auto& i : titles){
           if(i.second != ""){
               auto findFromRoot = musdata(i.first, i.second);
               auto mus = ROOTDB<<findFromRoot;
               if(mus.has_value()){
                   if(mus->empty()) continue;
                   musdata fromRoot = mus->front();
                   fromRoot.musicPath =
                   fs::relative(
                       fs::absolute(fs::path(ROOTDB.getRoot()).parent_path() / fromRoot.musicPath),
                       projectRoot
                   );
                   mds.push_back(fromRoot);
               }
           }
       }
   
       projectLocalDB->BuildProject(td, mds);
   }
