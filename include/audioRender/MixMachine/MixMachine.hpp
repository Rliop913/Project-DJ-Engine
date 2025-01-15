#pragma once
#include "MixTranslator.hpp"
#include "dbRoot.hpp"


class MixMachine{
private:
    unsigned long getMixSize(unsigned long frames);
public:
    std::vector<float> MixOuts;
    bool operator<<(const MixBinary<READ_MODE::READ_ONLY>& binary);
    bool mix(litedb& db);
    
    MixMachine(const unsigned int ch);
    ~MixMachine();
};

