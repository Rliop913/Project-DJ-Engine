#pragma once

#include <memory>

#include "FAUST_COMPRESSOR_manual.hpp"
#include "FAUST_DISTORTION_manual.hpp"
#include "FAUST_ECHO_manual.hpp"
#include "FAUST_EQ_manual.hpp"
#include "FAUST_FILTERS_manual.hpp"
#include "FAUST_FLANGER_manual.hpp"
#include "FAUST_OCS_FILTER_manual.hpp"
#include "FAUST_PANNER_manual.hpp"
#include "FAUST_PHASER_manual.hpp"
#include "FAUST_ROBOT_manual.hpp"
#include "FAUST_ROLL_manual.hpp"
#include "FAUST_TRANCE_manual.hpp"
#include "FAUST_VOL_manual.hpp"

#include "FrameCalc.hpp"
#include "musicDB.hpp"

#include "PDJE_EXPORT_SETTER.hpp"

/**
 * @brief the usable fx list
 *
 */
enum FXList {
    COMPRESSOR = 0,
    DISTORTION,
    ECHO,
    EQ,
    FILTER,
    FLANGER,
    OCSFILTER,
    PANNER,
    PHASER,
    ROBOT,
    ROLL,
    TRANCE,
    VOL
};

/**
 * @brief FX Controller class
 *
 */
class PDJE_API FXControlPanel {
  private:
    std::pair<bool, CompressorFAUSTMan> compressorPanel;
    std::pair<bool, DistortionFAUSTMan> distortionPanel;
    std::pair<bool, EchoFAUSTMan>       echoPanel;
    std::pair<bool, EQFAUSTMan>         eqPanel;
    std::pair<bool, FilterFAUSTMan>     filterPanel;
    std::pair<bool, FlangerFAUSTMan>    flangerPanel;
    std::pair<bool, OcsFilterFAUSTMan>  ocsFilterPanel;
    std::pair<bool, PannerFAUSTMan>     pannerPanel;
    std::pair<bool, PhaserFAUSTMan>     phaserPanel;
    std::pair<bool, RobotFAUSTMan>      robotPanel;
    std::pair<bool, RollFAUSTMan>       rollPanel;
    std::pair<bool, TranceFAUSTMan>     trancePanel;
    std::pair<bool, VolFAUSTMan>        volPanel;

    /**
     * @brief checks fxPanel activated and apply fx
     *
     * @tparam ManName FXPanel Type
     * @param pcm Faust Style float array
     * @param samples length of float array
     * @param man FXPanel Object
     */
    template <typename ManName>
    void
    checkAndUse(float **pcm, int samples, ManName &man)
    {
        if (man.first) {
            man.second.compute(samples, pcm, pcm);
        }
    }

  public:
    /**
     * @brief Construct a new FXControlPanel object
     *
     * @param sampleRate the sample rate. we use 48000
     */
    FXControlPanel(int sampleRate);
    /**
     * @brief Get the Arg Setter object
     *
     * @param fx the fx type
     * @return ARGSETTER the FX arg handler
     */
    ARGSETTER
    GetArgSetter(FXList fx);
    /**
     * @brief activate/deactivate FX
     *
     * @param fx the fx type
     * @param onoff activate / deactivate
     */
    void
    FX_ON_OFF(FXList fx, bool onoff);

    /**
     * @brief apply FX to pcm
     *
     * @param pcm the faust style PCM frame array
     * @param samples length of frame array
     */
    void
    addFX(float **pcm, int samples);

    /**
     * @brief check any FX is activated
     *
     * @return true , something is activated
     * @return false , nothing activated.
     */
    bool
    checkSomethingOn();
};

inline void
toFaustStylePCM(float **faustPCM, float *in, const unsigned long frameCount)
{
    float *op = in;
    float *lp = faustPCM[0];
    float *rp = faustPCM[1];
    for (int i = 0; i < frameCount; ++i) {
        *(lp++) = *(op++);
        *(rp++) = *(op++);
    }
}

inline void
toLRStylePCM(float **faustPCM, float *out, const unsigned long frameCount)
{
    float *op = out;
    float *lp = faustPCM[0];
    float *rp = faustPCM[1];
    for (int i = 0; i < frameCount; ++i) {
        *(op++) = *(lp++);
        *(op++) = *(rp++);
    }
}
