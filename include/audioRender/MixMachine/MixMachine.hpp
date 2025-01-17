#pragma once

#include <unordered_map>
#include <thread>
#include <optional>

#include "MixTranslator.hpp"
#include "dbRoot.hpp"
#include "Deck.hpp"
#include "Decoder.hpp"

using ID = long;

class MixMachine{
private:
    unsigned long getMixSize(unsigned long frames);
public:
    std::unordered_map<ID, std::vector<MixStruct>> Memorized;
    // std::vector<float> MixOuts;
    bool IDsort(const MixTranslator& binary);
    std::optional<std::vector<float>> mix(litedb& db, const BPM& bpms);
    
    MixMachine();
    ~MixMachine();
};

