#pragma once

#include <unordered_map>
#include <thread>
#include <optional>
#include <sstream>

#include "MixTranslator.hpp"
#include "dbRoot.hpp"
#include "EFFECTS.hpp"
#include "Decoder.hpp"
#include "BattleDj.hpp"


#define TRY(CODE)\
try\
{\
    CODE\
}\
catch(...)\
{\
    return false;\
}

enum InterpolateType{
    LINEAR,
    COSINE,
    CUBIC,
    FLAT
};

using ID = long;

struct EightPointValues{
    float vals[8] = {0, };
    EightPointValues(const std::string& rawData){
        std::stringstream sdata(rawData);
        std::string token;
        int counter = 0;
        while(std::getline(sdata, token, ',')){
            try
            {
                vals[counter++] = std::stof(token);
                if(counter > 7){
                    break;
                }
            }
            catch(...)
            {
                break;
            }
        }
    }
};


class MixMachine{
private:
    // FRAME_POS getMixSize(FRAME_POS frames);
public:
    std::unordered_map<ID, std::vector<MixStruct>> Memorized;
    bool IDsort(const MixTranslator& binary);
    bool mix(litedb& db, const BPM& bpms);
    
    std::vector<float> rendered_out;
    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data);

    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data, litedb& db);
    
    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data, std::vector<float>* Vec);
    
    template<typename FXtype>
    bool InterpolateInit(FXtype& FXvec, std::vector<float>*& PCMvec, MixStruct& ms){
        FXvec.emplace_back(PCMvec, ms.frame_in, ms.frame_out);

        TRY(
            FXvec.back().selectInterpolator =
            std::stoi(ms.RP.getFirst().cStr());
        )
        if(FXvec.back().selectInterpolator == InterpolateType::FLAT){
            TRY(
                FXvec.back().vZero =
                std::stof(ms.RP.getSecond().cStr());
            )
        }
        else{
            EightPointValues EPV(ms.RP.getSecond().cStr());
            FXvec.back().v1 = EPV.vals[0];
            FXvec.back().v2 = EPV.vals[1];
            FXvec.back().v3 = EPV.vals[2];
            FXvec.back().v4 = EPV.vals[3];
            FXvec.back().v5 = EPV.vals[4];
            FXvec.back().v6 = EPV.vals[5];
            FXvec.back().v7 = EPV.vals[6];
            FXvec.back().v8 = EPV.vals[7];
        }

        FXvec.back().frames = ms.frame_out - ms.frame_in;
        FXvec.back().timerActive = 0;
        return true;
    }

    MixMachine();
    ~MixMachine();
};

