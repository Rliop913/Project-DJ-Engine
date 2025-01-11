#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <string>

#include "Structs.hpp"
#include "Mix.hpp"

class BPM{
private:
    unsigned usable_threads;
public:
    std::vector<BpmStruct> bpmVec;
    bool getBpms(MIX& mixx);
    BPM();
    ~BPM();
};