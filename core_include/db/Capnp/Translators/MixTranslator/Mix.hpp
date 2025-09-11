#pragma once

#include <algorithm>
#include <mutex>
#include <thread>
#include <vector>

#include "FrameCalc.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
class BPM;

/**
 * @brief contains mixing datas
 *
 */
class PDJE_API MIX {
  private:
    unsigned int usable_threads;

  public:
    std::vector<MixStruct> mixVec;
    /**
     * @brief opens capnp binary data.
     *
     * @param Rptr gets mix binary capnp wrapper. this should be initialized
     * @return true
     * @return false
     */
    bool
    openMix(const MixBinaryCapnpData::Reader &Rptr);

    /**
     * @brief Preprocessing for mix data. This calculates the frame position of
     * the mixing data.
     *
     * @param bpmm bpm class
     * @return true
     * @return false
     */
    bool
    WriteFrames(BPM &bpmm);

    MIX();
    ~MIX();
};