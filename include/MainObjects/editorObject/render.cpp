#include "editorObject.hpp"

bool
editorObject::render(const std::u8string& trackTitle, litedb& ROOTDB)
{
    std::unordered_map<std::u8string, std::u8string> titles;
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
        titles[i.musicName] = u8"";
    }

    for(auto& i : titles){
        if(i.second != u8""){
            auto findFromRoot = musdata(i.first, i.second);
            auto mus = ROOTDB<<findFromRoot;
            if(mus.has_value()){
                if(mus->empty()) continue;
                musdata fromRoot = mus->front();
                try{
                    fromRoot.musicPath =
                    fs::relative(
                        fs::absolute(fs::path(ROOTDB.getRoot()).parent_path() / fromRoot.musicPath),
                        projectRoot
                    ).u8string();
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