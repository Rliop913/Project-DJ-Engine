
.. _program_listing_file_include_tests_manualAudioTest.cpp:

Program Listing for File manualAudioTest.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_manualAudioTest.cpp>` (``include\tests\manualAudioTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ManualMix.hpp"
   
   #include "miniaudio.h"
   #include "EFFECTS.hpp"
   
   auto idx = 0;
   FXControlPannel* fxcp;
   std::vector<float> Lvec(480);
   std::vector<float> Rvec(480);
   
   void
   idle_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
       // auto dvec = reinterpret_cast<std::vector<float>*>(pDevice->pUserData);
       auto dP = static_cast<ma_decoder*>(pDevice->pUserData);// dvec->data();
       ma_decoder_read_pcm_frames(dP, pOutput, frameCount, NULL);
       float* lp = Lvec.data();
       float* rp = Rvec.data();
       float* op = (float*)pOutput;
       for(int i=0; i<frameCount; ++i){
           *(lp++) = *(op++);
           *(rp++) = *(op++);
       }
       float *Fpcm[2] = {Lvec.data(), Rvec.data()};
       fxcp->addFX(Fpcm, frameCount);
       lp = Lvec.data();
       rp = Rvec.data();
       op = (float*)pOutput;
       
       for(int i=0; i<frameCount; ++i){
           *(op++) = *(lp++);
           *(op++) = *(rp++);
       }
   }
   
   #include <iostream>
   int main(int argc,  char *argv[])
   {
       for(int i=0;  i < 10; i+=3){
           std::cout << i  <<std::endl;
       }
   
       return 0;
   
       ma_device dev;
   
       fxcp = new FXControlPannel(48000);
   
       ma_decoder_config decconf = ma_decoder_config_init(ma_format_f32, 2, 48000);
       ma_decoder dec;
       ma_decoder_init_file("../../../../music/Over Time [bv7xMhvXJjc].wav", &decconf, &dec);
       ma_device_config devconf = ma_device_config_init(ma_device_type_playback);
       devconf.playback.format = ma_format_f32;
       devconf.playback.channels = 2;
       devconf.sampleRate = 48000;
       devconf.periodSizeInFrames = 480;
       devconf.dataCallback = idle_callback;
       devconf.performanceProfile = ma_performance_profile_low_latency;
       devconf.pUserData = (&dec);
   
       ma_device_init(NULL, &devconf, &dev);
       ma_device_start(&dev);
   
       getchar();
       
       fxcp->FX_ON_OFF(FXList::EQ, true);
       auto args = fxcp->GetArgSetter(FXList::EQ);
       for(auto i : args){
           std::cout << i.first <<std::endl;
       }
   
       args["EQPower"](-25);
       args["EQSelect"](2);
       getchar();
       for(int i=0; i<10;++i){
           args["EQPower"](25);
           getchar();
           args["EQPower"](0);
           getchar();
           args["EQPower"](-25);
           getchar();
       }
   
   
       getchar();
       delete fxcp;
       return 0;
   }
