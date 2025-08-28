#pragma once

#include "SoundTouch.h"

#include <optional>
#include <vector>

#include "MUSIC_CTR.hpp"
#include "MixTranslator.hpp"
/**
 * MASTER : Time-stretching the music without pitch shifting
 * RAW : Changes speed; pitch is affected
 * PITCH : pitch shifting
 *
 */
enum SoundTouchType { MASTER, RAW, PITCH };

/**
 * @brief Data structure required for implementing DJing FX
 *
 */
struct PDJE_API DJ_JOBS {
    unsigned long  sourcePoint;
    unsigned long  attachIn;
    unsigned long  attachOut;
    float          SPEED = 1.0;
    SoundTouchType STT;
    bool           getFromOrigin = true;
};

/**
 * @brief A class that implements the behavior of DJing FX
 *
 */
class BattleDj {
  private:
    soundtouch::SoundTouch *st;
    Decoder                *D;
    std::vector<DJ_JOBS>    jobs;

  public:
    BattleDj();
    ~BattleDj();
    /**
     * @brief initialize function gets soundtouch and decoder from MUSIC_CTR
     *
     * @param mc the MUSIC_CTR object
     * @return true
     * @return false
     */
    bool
    GetDataFrom(MUSIC_CTR &mc);
    /**
     * @brief the spin FX
     *
     * @param ms Mix object
     * @return true
     * @return false
     */
    bool
    Spin(MixStruct &ms);

    /**
     * @brief the Reverse play FX
     *
     * @param ms Mix object
     * @return true
     * @return false
     */
    bool
    Rev(MixStruct &ms);

    /**
     * @brief Scratch FX, can be used like a sampler.
     *
     * @param ms Mix object
     * @return true
     * @return false
     */
    bool
    Scratch(MixStruct &ms);

    /**
     * @brief pitch shifting FX.
     *
     * @param ms Mix object
     * @return true
     * @return false
     */
    bool
                                 Pitch(MixStruct &ms);
    std::optional<unsigned long> StartPos;

    /**
     * @brief apply FX
     *
     * @param Array PCM frames.
     * @return std::optional<SIMD_FLOAT*>
     */
    std::optional<SIMD_FLOAT *>
    operator<<(std::optional<SIMD_FLOAT *> Array);
};
