#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <string>

#include "FrameCalc.hpp"
#include "Mix.hpp"

/**
 * @brief Root class for BPM data
 * \dot
 * digraph PDJE_BPM{
 *      BPM -> BpmStruct;
 *      BpmStruct -> BpmFragment;
 * }
 * \enddot
 */
class BPM{
private:
    unsigned usable_threads;
public:
    BpmStruct bpmVec;
    bool getBpms(MIX& mixx);
    BPM();
    ~BPM();
};