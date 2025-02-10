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
// #include "FAUST_FILTERS.hpp"
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
    


    MixMachine();
    ~MixMachine();
};

