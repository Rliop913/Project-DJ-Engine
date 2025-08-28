#pragma once

#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "FrameCalc.hpp"
#include "Mix.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
/**
 * @brief Root class for BPM data
 * \dot
 * digraph PDJE_BPM{
 *      BPM -> BpmStruct;
 *      BpmStruct -> BpmFragment;
 * }
 * \enddot
 */
class PDJE_API BPM {
  private:
    unsigned usable_threads;

  public:
    BpmStruct bpmVec;
    bool
    getBpms(MIX &mixx);
    BPM();
    ~BPM();
};