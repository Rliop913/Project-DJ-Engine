#pragma once

#include <unordered_map>
#include <thread>
#include <optional>

#include "MixTranslator.hpp"
#include "dbRoot.hpp"
// #include "Deck.hpp"
#include "Decoder.hpp"
#include "BattleDj.hpp"
// #include "FAUST_FILTERS.hpp"
using ID = long;

class MixMachine{
private:
    FRAME_POS getMixSize(FRAME_POS frames);
public:
    std::unordered_map<ID, std::vector<MixStruct>> Memorized;
    bool IDsort(const MixTranslator& binary);
    bool mix(litedb& db, const BPM& bpms);
    
    std::vector<float> rendered_out;
    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data);
    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data, litedb& db);
    
    MixMachine();
    ~MixMachine();
};

