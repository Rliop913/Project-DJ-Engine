#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <string>

#include "FrameCalc.hpp"
#include "Mix.hpp"

class BPM{
private:
    unsigned usable_threads;
public:
    BpmStruct bpmVec;
    bool getBpms(MIX& mixx);
    BPM();
    ~BPM();
};