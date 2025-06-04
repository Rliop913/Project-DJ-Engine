#include "editorObject.hpp"

bool
editorObject::render(const std::string& trackTitle, litedb& ROOTDB)
{
    std::unordered_map<std::string, std::string> titles;
    auto td = makeTrackData(trackTitle, titles);
    
    std::vector<musdata> mds;
    for(auto& i : E_obj->musicHandle){
        mds.emplace_back();
        
        auto rendered = i.jsonh.render();
        mds.back().title = i.musicName;
        auto rdout = rendered->out();
        mds.back().bpmBinary.assign(rdout.begin(), rdout.end()); 
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
                ).string();
                mds.push_back(fromRoot);
            }
        }
    }

    projectLocalDB->BuildProject(td, mds);
}