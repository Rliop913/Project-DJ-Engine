#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <thread>
#include <mutex>
#include <optional>

#include "CapnpBinary.hpp"
#include "Mix.hpp"
#include "Bpm.hpp"

/**
 * @brief This Translate capnp data into cpp data sturct.
 * \dot
 * digraph MixTranslator{
 *      MixTranslator -> MIX;
 *      MixTranslator -> BPM;
 *      BPM -> BpmStruct;
 *      BpmStruct -> BpmFragment;
 * }
 * \enddot
 */
class MixTranslator{
private:
    unsigned int usable_threads = 0;
public:
    std::optional<MIX> mixs;
    std::optional<BPM> bpms;

    /**
     * @brief read capnp mix binary data and init MIX and BPM
     * 
     * @param binary the capnp Mix binary reader wrapper
     * @return true 
     * @return false 
     */
    bool Read(const CapReader<MixBinaryCapnpData>& binary);

    MixTranslator();
    ~MixTranslator();
};