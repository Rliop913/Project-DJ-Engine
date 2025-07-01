#include "editorObject.hpp"
#include <iostream>
bool
editorObject::render(const UNSANITIZED& trackTitle, litedb& ROOTDB)
{
    std::unordered_map<SANITIZED, SANITIZED> titles;
    auto td = makeTrackData(trackTitle, titles);
    
    std::vector<musdata> mds;
    for(auto& i : E_obj->musicHandle){
        mds.emplace_back();
        
        auto rendered = i.jsonh.render();
        mds.back().title = i.musicName;
        auto rdout = rendered->out();
        mds.back().bpmBinary.assign(rdout.begin(), rdout.end());
        auto tempCOMPOSER   = i.jsonh["COMPOSER"    ].get<SANITIZED>();
        auto tempPATH       = i.jsonh["PATH"        ].get<SANITIZED>();
        auto tempFIRST_BAR  = i.jsonh["FIRST_BAR"   ].get<DONT_SANITIZE>();
        std::cout << "DEBUGLINE: render.cpp:20   " << tempCOMPOSER <<std::endl;
        std::cout << "DEBUGLINE: render.cpp:21   " << tempPATH <<std::endl;
        std::cout << "DEBUGLINE: render.cpp:22   " << tempFIRST_BAR <<std::endl;
        
        mds.back().composer     = (tempCOMPOSER);
        mds.back().musicPath    = (tempPATH);
        mds.back().firstBar     = (tempFIRST_BAR);
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
            
            auto findFromRoot = musdata();
            findFromRoot.title = i.first;
            findFromRoot.composer = i.second;
            auto mus = ROOTDB << findFromRoot;
            if(mus.has_value()){
                if(mus->empty()) continue;
                musdata fromRoot = mus->front();
                try{
                    fromRoot.musicPath =
                    fs::relative(
                        fs::absolute(ROOTDB.getRoot().parent_path() / fromRoot.musicPath),
                        projectRoot
                    ).string();
                }
                catch(std::exception e){
                    RECENT_ERR = e.what();
                    return false;
                }
                mds.push_back(fromRoot);
            }
        }
    }

    return projectLocalDB->BuildProject(td, mds);
}