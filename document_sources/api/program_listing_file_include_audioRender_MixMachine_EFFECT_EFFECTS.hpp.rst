
.. _program_listing_file_include_audioRender_MixMachine_EFFECT_EFFECTS.hpp:

Program Listing for File EFFECTS.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_EFFECT_EFFECTS.hpp>` (``include/audioRender/MixMachine/EFFECT/EFFECTS.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <thread>
   
   #include "DeckData.hpp"
   #include "FAUST_COMPRESSOR.hpp"
   #include "FAUST_DISTORTION.hpp"
   #include "FAUST_ECHO.hpp"
   #include "FAUST_EQ.hpp"
   #include "FAUST_FILTERS.hpp"
   #include "FAUST_FLANGER.hpp"
   #include "FAUST_OCS_FILTER.hpp"
   #include "FAUST_PANNER.hpp"
   #include "FAUST_PHASER.hpp"
   #include "FAUST_ROBOT.hpp"
   #include "FAUST_ROLL.hpp"
   #include "FAUST_TRANCE.hpp"
   #include "FAUST_VOL.hpp"
   #include "FrameCalc.hpp"
   
   #include "PDJE_EXPORT_SETTER.hpp"
   
   template <typename Fclass> class FaustDType : public Fclass {
     private:
       std::vector<float> L;
       std::vector<float> R;
       SIMD_FLOAT        *originVec;
       unsigned long long startPos;
   
     public:
       int         count;
       FAUSTFLOAT *PTR[CHANNEL];
   
       FaustDType(SIMD_FLOAT        *vec,
                  unsigned long long start,
                  unsigned long long end)
           : startPos(start), originVec(vec)
       {
           count = end - start;
           L.resize(count);
           R.resize(count);
           PTR[0] = L.data();
           PTR[1] = R.data();
       }
       void
       CopyToFaust()
       {
           float *lp = L.data();
           float *rp = R.data();
           float *op = originVec->data();
           op += startPos * CHANNEL;
           for (int i = 0; i < count; ++i) {
               *(lp++) = *(op++);
               *(rp++) = *(op++);
           }
       }
       void
       WriteToOrigin()
       {
           float *lp = L.data();
           float *rp = R.data();
           float *op = originVec->data();
           op += startPos * CHANNEL;
           for (int i = 0; i < count; ++i) {
               *(op++) = *(lp++);
               *(op++) = *(rp++);
           }
       }
   };
   
   template <typename FaustClass> class FaustObject {
   
     public:
       FaustClass managingClass;
   
       template <typename FClass>
       void
       consume(std::vector<FaustDType<FClass>> &jobs)
       {
           for (auto &i : jobs) {
               i.CopyToFaust();
               i.template copySetting<FClass>(managingClass);
               managingClass.instanceClear();
               managingClass.compute(i.count, i.PTR, i.PTR);
               i.WriteToOrigin();
           }
       }
   };
   
   class FaustEffects {
     public:
       FaustObject<CompressorFAUST> compressor;
       FaustObject<DistortionFAUST> distortion;
       FaustObject<EchoFAUST>       echo;
       FaustObject<EQFAUST>         eq;
       FaustObject<FilterFAUST>     filter;
       FaustObject<FlangerFAUST>    flanger;
       FaustObject<OcsFilterFAUST>  ocsFilter;
       FaustObject<PannerFAUST>     panner;
       FaustObject<PhaserFAUST>     phaser;
       FaustObject<RollFAUST>       roll;
       FaustObject<TranceFAUST>     trance;
       FaustObject<VolFAUST>        vol;
       FaustObject<RobotFAUST>      robot;
       // public:
       std::vector<FaustDType<Compressor_PDJE>>  compressorData;
       std::vector<FaustDType<FaustInterpolate>> distortionData;
       std::vector<FaustDType<Echo_PDJE>>        echoData;
       std::vector<FaustDType<EQ_PDJE>>          eqData;
       std::vector<FaustDType<Filter_PDJE>>      filterData;
       std::vector<FaustDType<Flanger_PDJE>>     flangerData;
       std::vector<FaustDType<OcsFilter_PDJE>>   ocsFilterData;
       std::vector<FaustDType<Panner_PDJE>>      pannerData;
       std::vector<FaustDType<Phaser_PDJE>>      phaserData;
       std::vector<FaustDType<Roll_PDJE>>        rollData;
       std::vector<FaustDType<Trance_PDJE>>      tranceData;
       std::vector<FaustDType<FaustInterpolate>> volData;
       std::vector<FaustDType<Robot_PDJE>>       robotData;
   
       FaustEffects(int initSampleRate);
       void
       consumeAll();
   };
