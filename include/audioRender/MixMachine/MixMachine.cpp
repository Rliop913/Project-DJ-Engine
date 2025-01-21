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


bool
MixMachine::mix(litedb& db, const BPM& bpms)
{
    auto num_threads = Memorized.size();
    for(auto i : Memorized){// todo - impl this to threads
        auto MC = MUSIC_CTR();
        auto DJ = BattleDj();
        DJ.GetDataFrom(MC);
        for(auto j : i.second){
            switch (j.RP.getType()){
            case TypeEnum::BATTLE_DJ:
                {
                    switch (j.RP.getDetails()){
                    case DetailEnum::SPIN:
                        if(!DJ.Spin(j)){
                            continue;
                        }
                        break;
                    case DetailEnum::REV:
                        if(!DJ.Rev(j)){
                            continue;
                        }
                        break;
                    case DetailEnum::PITCH:
                        if(!DJ.Pitch(j)){
                            continue;
                        }
                        break;
                    case DetailEnum::SCRATCH:
                        if(!DJ.Scratch(j)){
                            continue;
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
            case TypeEnum::LOAD:
                {
                    if(!MC.setLOAD(j.RP, db, j.frame_in)){
                        return false;
                    }
                    DJ.StartPos = j.frame_in;

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
        
        auto result = DJ << MC.Execute(bpms, &rendered_out);
        return result.has_value();
    }

    //todo - implement mix
    return true;
}

MixMachine::~MixMachine()
{
    
}