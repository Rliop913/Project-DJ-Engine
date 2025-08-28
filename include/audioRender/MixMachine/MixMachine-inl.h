/**
 * @file MixMachine-inl.h
 * @brief SIMD-optimized implementation for integrating PCM frames into a final
 * mix.
 */
#include "MixMachine.hpp"

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "MixMachine-inl.h"
#include "hwy/foreach_target.h"

#include "hwy/base.h"
#include <hwy/highway.h>

namespace HWY_NAMESPACE {

/**
 * @brief Integrates a temporary PCM buffer into the main rendered output using
 * SIMD instructions.
 *
 * This function safely mixes a temporary vector of PCM data (`tempVec`) into
 * the final output buffer (`rendered_out`). It uses a mutex to protect access
 * to the output buffer and leverages the Highway library for SIMD-accelerated
 * audio mixing.
 *
 * @param[in] tempVec The temporary buffer of PCM frames to be mixed.
 * @param[in] renderLock A mutex to ensure thread-safe access to the output
 * buffer.
 * @param[in,out] rendered_out The final output buffer where the PCM data is
 * mixed.
 * @param[in] MC A pointer to the `MUSIC_CTR` object, which provides context for
 * the mixing operation.
 */
HWY_ATTR
void
INTEGRATE_PCM_SIMD(SIMD_FLOAT         &tempVec,
                   std::mutex         &renderLock,
                   std::vector<float> &rendered_out,
                   MUSIC_CTR         *&MC)
{
    const hwy::HWY_NAMESPACE::ScalableTag<float> hwyFTag;
    auto laneSize = hwy::HWY_NAMESPACE::Lanes(hwyFTag);
    auto times    = tempVec.size() / laneSize;
    auto remained = tempVec.size() % laneSize;

    auto Tptr = tempVec.data();
    {
        std::lock_guard<std::mutex> locks(renderLock);
        if (rendered_out.size() < (MC->QDatas.pos.back().Gidx * CHANNEL)) {
            rendered_out.resize((MC->QDatas.pos.back().Gidx * CHANNEL));
        }
        auto Rptr =
            rendered_out.data() + (MC->QDatas.pos.front().Gidx * CHANNEL);

        for (size_t L = 0; L < times; ++L) {
            auto Tsimd = hwy::HWY_NAMESPACE::Load(hwyFTag, Tptr);
            auto Rsimd = hwy::HWY_NAMESPACE::LoadU(hwyFTag, Rptr);
            hwy::HWY_NAMESPACE::StoreU(Rsimd + Tsimd, hwyFTag, Rptr);
            Tptr += laneSize;
            Rptr += laneSize;
        }
        for (size_t REM = 0; REM < remained; ++REM) {
            (*(Rptr++)) += (*(Tptr++));
        }
    }
}

} // namespace HWY_NAMESPACE
