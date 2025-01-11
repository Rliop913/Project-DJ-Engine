#pragma once

#include <algorithm>
#include <thread>
#include <vector>
#include <mutex>

#include "Structs.hpp"

class BPM;

class MIX{
private:
    unsigned int usable_threads;
public:
    std::vector<MixStruct> mixVec;
    bool openMix(MixBinaryCapnpData::Reader* Rptr);
    bool WriteFrames(BPM& bpmm);

    MIX();
    ~MIX();
};