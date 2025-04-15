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
PDJE_JSONHandler::add(
    TypeEnum type, 
    DetailEnum details,
    int ID,
    const std::string& first,
    const std::string& second,
    const std::string& third,
    long long bar,
    long long beat,
    long long separate,
    long long Ebar,
    long long Ebeat,
    long long Eseparate
)
{
    nj tempMix = {
        {"type",        static_cast<int>(type)},
        {"details",     static_cast<int>(details)},
        {"ID",          ID},
        {"first",       first},
        {"second",      second},
        {"third",       third},
        {"bar",         bar},
        {"beat",        beat},
        {"separate",    separate},
        {"Ebar",        Ebar},
        {"Ebeat",       Ebeat},
        {"Eseparate",   Eseparate}
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
PDJE_JSONHandler::deleteLine(
    TypeEnum type, 
    DetailEnum details,
    int ID,
    const std::string& first,
    const std::string& second,
    const std::string& third,
    long long bar,
    long long beat,
    long long separate,
    long long Ebar,
    long long Ebeat,
    long long Eseparate
)
{
    std::vector<int> targetIDX;
    try{
        for(unsigned long long i=0; i < ROOT[PDJEARR].size(); ++i){
            auto Target = ROOT[PDJEARR].at(i);
            if(Target["type"]       != type                             )   continue;
            if(Target["details"]    != details                          )   continue;
            if(Target["ID"]         != ID       && ID           != -1   )   continue;
            if(Target["first"]      != first    && first        != ""   )   continue;
            if(Target["second"]     != second   && second       != ""   )   continue;
            if(Target["third"]      != third    && third        != ""   )   continue;
            if(Target["bar"]        != bar      && bar          != -1   )   continue;
            if(Target["beat"]       != beat     && beat         != -1   )   continue;
            if(Target["separate"]   != separate && separate     != -1   )   continue;
            if(Target["Ebar"]       != Ebar     && Ebar         != -1   )   continue;
            if(Target["Ebeat"]      != Ebeat    && Ebeat        != -1   )   continue;
            if(Target["Eseparate"]  != Eseparate&& Eseparate    != -1   )   continue;
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
    std::function<void(
        TypeEnum type,
        DetailEnum details,
        int ID,
        const std::string& first,
        const std::string& second,
        const std::string& third,
        long long bar,
        long long beat,
        long long separate,
        long long Ebar,
        long long Ebeat,
        long long Eseparate
    )> jsonCallback
)
{
    if(!ROOT.contains(PDJEARR)){
        return;
    }
    for(auto& i : ROOT[PDJEARR]){
        jsonCallback(
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
        );
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