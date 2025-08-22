
.. _program_listing_file_include_audioRender_ManualMix_ManualMix.cpp:

Program Listing for File ManualMix.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualMix.cpp>` (``include/audioRender/ManualMix/ManualMix.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ManualMix.hpp"
   
   FXControlPannel::FXControlPannel(int sampleRate)
   {
       compressorPannel.first  = false;
       distortionPannel.first  = false;
       echoPannel.first        = false;
       eqPannel.first          = false;
       filterPannel.first      = false;
       flangerPannel.first     = false;
       ocsFilterPannel.first   = false;
       pannerPannel.first      = false;
       phaserPannel.first      = false;
       robotPannel.first       = false;
       rollPannel.first        = false;
       trancePannel.first      = false;
       volPannel.first         = false;
   
       compressorPannel.second .init(sampleRate);
       distortionPannel.second .init(sampleRate);
       echoPannel.second       .init(sampleRate);
       eqPannel.second         .init(sampleRate);
       filterPannel.second     .init(sampleRate);
       flangerPannel.second    .init(sampleRate);
       ocsFilterPannel.second  .init(sampleRate);
       pannerPannel.second     .init(sampleRate);
       phaserPannel.second     .init(sampleRate);
       robotPannel.second      .init(sampleRate);
       rollPannel.second       .init(sampleRate);
       trancePannel.second     .init(sampleRate);
       volPannel.second        .init(sampleRate);
   }
   
   void
   FXControlPannel::FX_ON_OFF(FXList fx, bool onoff)
   {
       switch (fx){
           
       case FXList::COMPRESSOR:
           compressorPannel.first  = onoff;
           break;
   
       case FXList::DISTORTION:
           distortionPannel.first  = onoff;
           break;
   
       case FXList::ECHO:
           echoPannel.first        = onoff;
           break;
   
       case FXList::EQ:
           eqPannel.first          = onoff;
           break;
   
       case FXList::FILTER:
           filterPannel.first      = onoff;
           break;
   
       case FXList::FLANGER:
           flangerPannel.first     = onoff;
           break;
   
       case FXList::OCSFILTER:
           ocsFilterPannel.first   = onoff;
           break;
   
       case FXList::PANNER:
           pannerPannel.first      = onoff;
           break;
   
       case FXList::PHASER:
           phaserPannel.first      = onoff;
           break;
   
       case FXList::ROBOT:
           robotPannel.first       = onoff;
           break;
   
       case FXList::ROLL:
           rollPannel.first        = onoff;
           break;
   
       case FXList::TRANCE:
           trancePannel.first      = onoff;
           break;
   
       case FXList::VOL:
           volPannel.first         = onoff;
           break;
   
       default:
           break;
       }
   }
   
   ARGSETTER
   FXControlPannel::GetArgSetter(FXList fx)
   {
       switch (fx)
       {
       case FXList::COMPRESSOR:
           return compressorPannel.second.makeArgSetter();
           break;
   
       case FXList::DISTORTION:
           return distortionPannel.second.makeArgSetter();
           break;
   
       case FXList::ECHO:
           return echoPannel.second.makeArgSetter();
           break;
   
       case FXList::EQ:
           return eqPannel.second.makeArgSetter();
           break;
   
       case FXList::FILTER:
           return filterPannel.second.makeArgSetter();
           break;
   
       case FXList::FLANGER:
           return flangerPannel.second.makeArgSetter();
           break;
   
       case FXList::OCSFILTER:
           return ocsFilterPannel.second.makeArgSetter();
           break;
   
       case FXList::PANNER:
           return pannerPannel.second.makeArgSetter();
           break;
   
       case FXList::PHASER:
           return phaserPannel.second.makeArgSetter();
           break;
   
       case FXList::ROBOT:
           return robotPannel.second.makeArgSetter();
           break;
   
       case FXList::ROLL:
           return rollPannel.second.makeArgSetter();
           break;
   
       case FXList::TRANCE:
           return trancePannel.second.makeArgSetter();
           break;
   
       case FXList::VOL:
           return volPannel.second.makeArgSetter();
           break;
           
       default:
           return ARGSETTER();
           break;
       }
   }
   
   void
   FXControlPannel::addFX(float** pcm, int samples)
   {
       checkAndUse(pcm, samples, compressorPannel);
       checkAndUse(pcm, samples, distortionPannel);
       checkAndUse(pcm, samples, echoPannel);
       checkAndUse(pcm, samples, eqPannel);
       checkAndUse(pcm, samples, filterPannel);
       checkAndUse(pcm, samples, flangerPannel);
       checkAndUse(pcm, samples, ocsFilterPannel);
       checkAndUse(pcm, samples, pannerPannel);
       checkAndUse(pcm, samples, phaserPannel);
       checkAndUse(pcm, samples, robotPannel);
       checkAndUse(pcm, samples, rollPannel);
       checkAndUse(pcm, samples, trancePannel);
       checkAndUse(pcm, samples, volPannel);
       
   }
   
   bool
   FXControlPannel::checkSomethingOn()
   {
       return
           compressorPannel.first ||
           distortionPannel.first ||
           echoPannel.first ||
           eqPannel.first ||
           filterPannel.first ||
           flangerPannel.first ||
           ocsFilterPannel.first ||
           pannerPannel.first ||
           phaserPannel.first ||
           robotPannel.first ||
           rollPannel.first ||
           trancePannel.first ||
           volPannel.first;
   }
