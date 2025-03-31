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
    for(auto& i: Memorized){
        renderPool.emplace_back([i, this, &db, &bpms](){
                
            auto MC = new MUSIC_CTR();
            auto DJ = new BattleDj();
            auto FX = new FaustEffects(SAMPLERATE);
            
            SIMD_FLOAT tempVec;
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

            //임시 믹싱 코드 더 효율적 구현 필요 -> HIGHWAY 사용 예정 -> 적용완료
            auto result = (*DJ) << MC->Execute(bpms, &tempVec);
            FX->consumeAll();
            if(!result.has_value()){
                FLAG_SOMETHING_WRONG_ID = i.first;
                return;
            }
            const hn::ScalableTag<float> hwyFTag;
            auto laneSize = hn::Lanes(hwyFTag);
            auto times = tempVec.size() / laneSize;
            auto remained = tempVec.size() % laneSize;

            auto Tptr = tempVec.data();
            {
                std::lock_guard<std::mutex> locks(renderLock);
                if(rendered_out.size() < (MC->QDatas.pos.back().Gidx * CHANNEL)){
                    rendered_out.resize((MC->QDatas.pos.back().Gidx * CHANNEL));
                }
                auto Rptr = rendered_out.data() + (MC->QDatas.pos.front().Gidx * CHANNEL);

                for(size_t L = 0; L < times; ++L){
                    auto Tsimd = hn::Load(hwyFTag, Tptr);
                    auto Rsimd = hn::LoadU(hwyFTag, Rptr);
                    hn::StoreU(Rsimd + Tsimd, hwyFTag, Rptr);
                    Tptr += laneSize;
                    Rptr += laneSize;
                }
                for(size_t REM = 0; REM < remained; ++REM){
                    (*(Rptr++)) += (*(Tptr++));
                }
            }
            // PFloat = result.value()->data();

            // for(unsigned long q = MC->QDatas.pos.front().Gidx; 
            //     q < (MC->QDatas.pos.back().Gidx - MC->QDatas.pos.front().Gidx) * CHANNEL;
            //         ++q){
                
            //     (*Rptr) += (*Tptr);
            //     ++Rptr;
            //     ++Tptr;
            // }
            
            delete MC;
            delete DJ;
            delete FX;
        });
    }
    for(auto& pool: renderPool){
        pool.join();
    }
    if(FLAG_SOMETHING_WRONG_ID != FLAG_ALL_IS_OK){
        return false;
    }
    //todo - implement mix
    return true;
}

MixMachine::~MixMachine()
{
    
}