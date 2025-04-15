#include "jsonWrapper.hpp"


bool
PDJE_JSONHandler::load(const std::string& path)
{
    auto filepath = fs::path(path); 
    if(fs::exists(filepath)){
        if(fs::is_regular_file(filepath)){
            std::ifstream jfile(filepath);
            
            if(!jfile.is_open()) return false;

            try{ jfile >> ROOT; }
            catch(...){ return false; }

            jfile.close();
        }
        else{
            return false;
        }
    }
    else{
        fs::create_directories(filepath.parent_path());
        std::ofstream jfile(filepath);
        if(!jfile.is_open()) return false;
        jfile.close();
    }

    if(!ROOT.contains(PDJEARR)){
        ROOT[PDJEARR] = nj::array();
    }

    return true;

}

bool
PDJE_JSONHandler::add(const MixArgs& args)
{
    nj tempMix = {
        {"type",        static_cast<int>(args.type)},
        {"details",     static_cast<int>(args.details)},
        {"ID",          args.ID},
        {"first",       args.first},
        {"second",      args.second},
        {"third",       args.third},
        {"bar",         args.bar},
        {"beat",        args.beat},
        {"separate",    args.separate},
        {"Ebar",        args.Ebar},
        {"Ebeat",       args.Ebeat},
        {"Eseparate",   args.Eseparate}
    };
    if(!ROOT.contains(PDJEARR)){
        return false;
    }
    ROOT[PDJEARR].push_back(tempMix);
    return true;
}


bool
PDJE_JSONHandler::save(const std::string& path)
{
    std::ofstream jfile(path);
    if(jfile.is_open()){
        jfile << std::setw(4) << ROOT;
        return true;
    }
    else{
        return false;
    }
}

int
PDJE_JSONHandler::deleteLine(const MixArgs& args, bool skipType, bool skipDetail
)
{
    std::vector<int> targetIDX;
    try{
        for(unsigned long long i=0; i < ROOT[PDJEARR].size(); ++i){
            auto Target = ROOT[PDJEARR].at(i);
            if(Target["type"]       != args.type     && !skipType                   )   continue;
            if(Target["details"]    != args.details  && !skipDetail                 )   continue;
            if(Target["ID"]         != args.ID       && args.ID             != -1   )   continue;
            if(Target["first"]      != args.first    && args.first          != ""   )   continue;
            if(Target["second"]     != args.second   && args.second         != ""   )   continue;
            if(Target["third"]      != args.third    && args.third          != ""   )   continue;
            if(Target["bar"]        != args.bar      && args.bar            != -1   )   continue;
            if(Target["beat"]       != args.beat     && args.beat           != -1   )   continue;
            if(Target["separate"]   != args.separate && args.separate       != -1   )   continue;
            if(Target["Ebar"]       != args.Ebar     && args.Ebar           != -1   )   continue;
            if(Target["Ebeat"]      != args.Ebeat    && args.Ebeat          != -1   )   continue;
            if(Target["Eseparate"]  != args.Eseparate&& args.Eseparate      != -1   )   continue;
            targetIDX.push_back(i);
        }
        for(auto i : targetIDX | vs::reverse){
            ROOT[PDJEARR].erase(i);
        }
    }
    catch(...){
        return 0;
    }
    return static_cast<int>(targetIDX.size());
}


void
PDJE_JSONHandler::getAll(
    std::function<void(const MixArgs& args)> jsonCallback
)
{
    if(!ROOT.contains(PDJEARR)){
        return;
    }
    for(auto& i : ROOT[PDJEARR]){
        MixArgs tempargs{
            i["type"],
            i["details"],
            i["ID"],
            i["first"],
            i["second"],
            i["third"],
            i["bar"],
            i["beat"],
            i["separate"],
            i["Ebar"],
            i["Ebeat"],
            i["Eseparate"]
        };
        jsonCallback(tempargs);
    }
}


bool
PDJE_JSONHandler::deleteFile(const std::string& path)
{
    try{ return fs::remove_all(path) > 0; }
    catch(...) { return false; }
}


std::unique_ptr<CapWriter<MixBinaryCapnpData>>
PDJE_JSONHandler::render()
{
    try{
        auto tempMixBin = std::make_unique<CapWriter<MixBinaryCapnpData>>();
        tempMixBin->makeNew();
        auto rootsz = ROOT[PDJEARR].size();
        auto filler = tempMixBin->Wp->initDatas(rootsz);
        for(std::size_t i=0; i<rootsz; ++i){
            auto target = ROOT[PDJEARR].at(i);
            filler[i].setType(target["type"]);
            filler[i].setDetails(target["details"]);
            filler[i].setId(target["ID"]);
            filler[i].setFirst(target["first"].get<std::string>());
            filler[i].setSecond(target["second"].get<std::string>());
            filler[i].setThird(target["third"].get<std::string>());
            filler[i].setBar(target["bar"]);
            filler[i].setBeat(target["beat"]);
            filler[i].setSeparate(target["separate"]);
            filler[i].setEbar(target["Ebar"]);
            filler[i].setEbeat(target["Ebeat"]);
            filler[i].setEseparate(target["Eseparate"]);
        }

        return tempMixBin;
    }
    catch(...){
        return nullptr;
    }
}