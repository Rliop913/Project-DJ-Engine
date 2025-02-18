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
        auto MC = new MUSIC_CTR();
        auto DJ = new BattleDj();
        auto FX = new FaustEffects(SAMPLERATE);
        
        std::vector<float> tempVec;
        DJ->GetDataFrom(*MC);
        for(auto j : i.second){
            switch (j.RP.getType()){

            case TypeEnum::BATTLE_DJ: 
                if(TypeWorks<TypeEnum::BATTLE_DJ>(j, *DJ)) break;
                else continue;

            case TypeEnum::LOAD: 
                if(TypeWorks<TypeEnum::LOAD>(j, *MC, db)
                    && TypeWorks<TypeEnum::LOAD>(j, *DJ)) break;
                else continue;
                
            case TypeEnum::CONTROL:
                if(TypeWorks<TypeEnum::CONTROL>(j, *MC)) break;
                else continue;

            case TypeEnum::UNLOAD:
                if(TypeWorks<TypeEnum::UNLOAD>(j, *MC)) break;
                else continue;

            case TypeEnum::EQ:
                if(TypeWorks<TypeEnum::EQ>(j, *FX, &tempVec)) break;
                else continue;

            case TypeEnum::COMPRESSOR:
                if(TypeWorks<TypeEnum::COMPRESSOR>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::FILTER:
                if(TypeWorks<TypeEnum::FILTER>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::DISTORTION:
                if(TypeWorks<TypeEnum::DISTORTION>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::ECHO:
                if(TypeWorks<TypeEnum::ECHO>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::FLANGER:
                if(TypeWorks<TypeEnum::FLANGER>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::OSC_FILTER:
                if(TypeWorks<TypeEnum::OSC_FILTER>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::PANNER:
                if(TypeWorks<TypeEnum::PANNER>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::PHASER:
                if(TypeWorks<TypeEnum::PHASER>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::ROLL:
                if(TypeWorks<TypeEnum::ROLL>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::TRANCE:
                if(TypeWorks<TypeEnum::TRANCE>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::ROBOT:
                if(TypeWorks<TypeEnum::ROBOT>(j, *FX, &tempVec)) break;
                else continue;
            
            case TypeEnum::VOL:
                if(TypeWorks<TypeEnum::VOL>(j, *FX, &tempVec)) break;
                else continue;
            
            default:
                break;
            }
        }

        //임시 믹싱 코드 더 효율적 구현 필요 -> HIGHWAY 사용 예정
        auto result = (*DJ) << MC->Execute(bpms, &tempVec);
        // PFloat = result.value()->data();
        FX->consumeAll();
        if(!result.has_value()){
            return false;
        }
        if(rendered_out.size() < (MC->QDatas.pos.back().Gidx * CHANNEL)){
            rendered_out.resize((MC->QDatas.pos.back().Gidx * CHANNEL));
        }
        auto Rptr = rendered_out.data() + (MC->QDatas.pos.front().Gidx * CHANNEL);
        auto Tptr = tempVec.data();
        for(unsigned long i = MC->QDatas.pos.front().Gidx; i < (MC->QDatas.pos.back().Gidx - MC->QDatas.pos.front().Gidx) * CHANNEL; ++i){
            
            (*Rptr) += (*Tptr);
            ++Rptr;
            ++Tptr;
        }
        
        delete MC;
        delete DJ;
        delete FX;
    }

    //todo - implement mix
    return true;
}

MixMachine::~MixMachine()
{
    
}