#pragma once

#include <unordered_map>
#include <thread>

#include "MixTranslator.hpp"
#include "dbRoot.hpp"
#include "Deck.hpp"

using ID = long;

class MixMachine{
private:
    unsigned int num_channel = 2;
    unsigned long getMixSize(unsigned long frames);
public:
    std::unordered_map<ID, std::vector<MBData::Reader>> Memorized;
    std::vector<float> MixOuts;
    bool operator<<(const MixBinary<READ_MODE::READ_ONLY>& binary);
    bool mix(litedb& db);
    
    MixMachine(const unsigned int ch);
    ~MixMachine();
};

