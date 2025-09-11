
.. _program_listing_file_core_include_audioRender_ManualMix_ManualMix.hpp:

Program Listing for File ManualMix.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_audioRender_ManualMix_ManualMix.hpp>` (``core_include\audioRender\ManualMix\ManualMix.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

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
   
       template <typename ManName>
       void
       checkAndUse(float **pcm, int samples, ManName &man)
       {
           if (man.first) {
               man.second.compute(samples, pcm, pcm);
           }
       }
   
     public:
       FXControlPanel(int sampleRate);
       ARGSETTER
       GetArgSetter(FXList fx);
       void
       FX_ON_OFF(FXList fx, bool onoff);
   
       void
       addFX(float **pcm, int samples);
   
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
