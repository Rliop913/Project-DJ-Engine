#pragma once

#include <unordered_map>
#include <thread>
#include <optional>

#include "MixTranslator.hpp"
#include "dbRoot.hpp"
#include "Deck.hpp"
#include "Decoder.hpp"
#include "BattleDj.hpp"
using ID = long;

class MixMachine{
private:
    unsigned long getMixSize(unsigned long frames);
public:
    std::unordered_map<ID, std::vector<MixStruct>> Memorized;
    bool IDsort(const MixTranslator& binary);
    bool mix(litedb& db, const BPM& bpms);
    
    std::vector<float> rendered_out;

    MixMachine();
    ~MixMachine();
};

