
/**
 * @file MusicControlPanel-inl.h
 * @brief SIMD-optimized implementation for mixing PCM frames from multiple
 * sources.
 */
#include "MusicControlPanel.hpp"
#include "PDJE_Benchmark.hpp"
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "MusicControlPanel-inl.h"
#include "hwy/foreach_target.h"
#include <hwy/highway.h>

namespace HWY_NAMESPACE {

/**
 * @brief Retrieves and mixes PCM frames from active audio decks using SIMD
 * instructions.
 *
 * This function reads audio data from each active deck, applies any configured
 * Faust effects, and mixes the resulting audio into a single output buffer. The
 * mixing process is accelerated using the Highway SIMD library.
 *
 * @param[out] tempFrames A buffer to temporarily store raw PCM frames from a
 * decoder.
 * @param[out] L A buffer to store the left channel audio data for Faust
 * processing.
 * @param[out] R A buffer to store the right channel audio data for Faust
 * processing.
 * @param[in,out] FaustStyle A pointer array to the left and right channel
 * buffers (`L` and `R`).
 * @param[in] deck A map of loaded music decks to be processed.
 * @param[in,out] array The output buffer where the final mixed PCM frames will
 * be stored.
 * @param[in] FrameSize The number of PCM frames to process.
 * @return `true` if the frames were processed and mixed successfully, `false`
 * otherwise.
 */
HWY_ATTR
bool
GetPCMFramesSIMD(SIMD_FLOAT         &tempFrames,
                 std::vector<float> &L,
                 std::vector<float> &R,
                 float             **FaustStyle,
                 LOADS              &deck,
                 float              *array,
                 const unsigned long FrameSize)
{
    const unsigned long long RAWFrameSize = FrameSize * CHANNEL;

    tempFrames.resize(RAWFrameSize);
    L.resize(FrameSize);
    R.resize(FrameSize);
    FaustStyle[0] = L.data();
    FaustStyle[1] = R.data();
    const hwy::HWY_NAMESPACE::ScalableTag<float> hwyFTag;
    auto laneSize = hwy::HWY_NAMESPACE::Lanes(hwyFTag);
    auto times    = RAWFrameSize / laneSize;
    auto remained = RAWFrameSize % laneSize;

    // SIMD_FLOAT solaVector;
    PREDICT prd;
    for (auto &i : deck) {
        if (i.second.play) {
            if (!i.second.pb->Pop(prd)) {
                continue; // skip if not exists
            }

            toFaustStylePCM(FaustStyle, prd.predict_fragment.data(), FrameSize);
            i.second.fxP->addFX(FaustStyle, FrameSize);
            toLRStylePCM(FaustStyle, tempFrames.data(), FrameSize);

            float *opoint = array;
            float *tpoint = tempFrames.data();

            for (size_t j = 0; j < times; ++j) {
                auto simdtemp   = hwy::HWY_NAMESPACE::Load(hwyFTag, tpoint);
                auto simdorigin = hwy::HWY_NAMESPACE::LoadU(hwyFTag, opoint);
                auto res        = simdtemp + simdorigin;
                hwy::HWY_NAMESPACE::StoreU(res, hwyFTag, opoint);
                opoint += laneSize;
                tpoint += laneSize;
            }

            for (size_t j = 0; j < remained; ++j) {
                (*(opoint++)) += (*(tpoint++));
            }
        }
    }
    return true;
}
} // namespace HWY_NAMESPACE
