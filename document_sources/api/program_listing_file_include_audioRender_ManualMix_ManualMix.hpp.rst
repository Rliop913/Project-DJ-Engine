
.. _program_listing_file_include_audioRender_ManualMix_ManualMix.hpp:

Program Listing for File ManualMix.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualMix.hpp>` (``include\audioRender\ManualMix\ManualMix.hpp``)

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
   
   #include "musicDB.hpp"
   #include "FrameCalc.hpp"
   
   #include "PDJE_EXPORT_SETTER.hpp"
   
   enum FXList{
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
   
   class PDJE_API FXControlPannel{
   private:
       std::pair<bool, CompressorFAUSTMan>  compressorPannel;
       std::pair<bool, DistortionFAUSTMan>  distortionPannel;
       std::pair<bool, EchoFAUSTMan>        echoPannel;
       std::pair<bool, EQFAUSTMan>          eqPannel;
       std::pair<bool, FilterFAUSTMan>      filterPannel;
       std::pair<bool, FlangerFAUSTMan>     flangerPannel;
       std::pair<bool, OcsFilterFAUSTMan>   ocsFilterPannel;
       std::pair<bool, PannerFAUSTMan>      pannerPannel;
       std::pair<bool, PhaserFAUSTMan>      phaserPannel;
       std::pair<bool, RobotFAUSTMan>       robotPannel;
       std::pair<bool, RollFAUSTMan>        rollPannel;
       std::pair<bool, TranceFAUSTMan>      trancePannel;
       std::pair<bool, VolFAUSTMan>         volPannel;
   
       template<typename ManName>
       void
       checkAndUse(float** pcm, int samples, ManName& man)
       {
           if(man.first){
               man.second.compute(samples, pcm, pcm);
           }
       }
       
   public:
       FXControlPannel(int sampleRate);
       ARGSETTER
       GetArgSetter(FXList fx);
       void FX_ON_OFF(FXList fx, bool onoff);
   
       void addFX(float** pcm, int samples);
   
       bool checkSomethingOn();
   };
   
   
   
   inline
   void 
   toFaustStylePCM(float** faustPCM, float* in, const unsigned long frameCount)
   {
       float* op = in;
       float* lp = faustPCM[0];
       float* rp = faustPCM[1];
       for(int i=0; i<frameCount; ++i){
           *(lp++) = *(op++);
           *(rp++) = *(op++);
       }
   }
   
   inline
   void 
   toLRStylePCM(float** faustPCM, float* out, const unsigned long frameCount)
   {
       float* op = out;
       float* lp = faustPCM[0];
       float* rp = faustPCM[1];
       for(int i=0; i<frameCount; ++i){
           *(op++) = *(lp++);
           *(op++) = *(rp++);
       }
   }
