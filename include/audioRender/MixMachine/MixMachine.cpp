#include "MixMachine.hpp"


MixMachine::MixMachine()
{
}

bool
MixMachine::IDsort(const MixTranslator& tr)
{
    try
    {    
        for(auto i : tr.mixs.value().mixVec){
            long id = i.RP.getId();
            
            if(Memorized.find(id) == Memorized.end()){
                Memorized[id] = std::vector<MixStruct>();
            }
            Memorized[id].push_back(i);
        }
        
        return true;
    }
    catch(...)
    {
        
        return false;
    }
}


std::optional<std::vector<float>>
MixMachine::mix(litedb& db, const BPM& bpms)
{
    auto num_threads = Memorized.size();
    for(auto i : Memorized){// todo - impl this to threads
        auto MC = MUSIC_CTR();
        for(auto j : i.second){
            switch (j.RP.getType()){
            case TypeEnum::LOAD:
                {
                    musdata md;
                    md.title = j.RP.getFirst();
                    md.composer = j.RP.getSecond();
                    md.bpm = std::stod(j.RP.getThird().cStr());
                    MC.originBpm = md.bpm;
                    auto searchRes = db << md;
                    if(!searchRes.has_value()){
                        return std::nullopt;
                    }
                    MC.songPath = searchRes.value()[0].musicPath;
                    MC.FirstBarPos = std::stoul(searchRes.value()[0].firstBar);
                    MC.StartPos = j.frame_in;
                }
                break;
            case TypeEnum::CONTROL:
                {
                    MC.PausePos = j.frame_in;
                }
                break;
            case TypeEnum::UNLOAD:
                {
                    MC.FullPos = j.frame_in;
                }
                break;
            
            default:
                break;
            }
        }
        return MC.Execute(bpms);
    }

    //todo - implement mix

}

MixMachine::~MixMachine()
{
    
}