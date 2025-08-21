
.. _program_listing_file_include_MainObjects_audioPlayer_audioCallbacks.cpp:

Program Listing for File audioCallbacks.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_audioPlayer_audioCallbacks.cpp>` (``include\MainObjects\audioPlayer\audioCallbacks.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "audioCallbacks.hpp"
   #include "FrameCalc.hpp"
   #include <cstring>
   
   std::optional<float*>
   audioEngineDataStruct::getNowfPointer(const unsigned long frameCount)
   {
       if((nowCursor + frameCount) > maxCursor){
           return std::nullopt;
       }
       else{
           return pcmDataPoint->data() + (nowCursor * CHANNEL);
       }
   }
   
   void
   audioEngineDataStruct::CountUp(const unsigned long frameCount)
   {
       nowCursor += frameCount;
       consumedFrames += frameCount;
   }
   
   void
   audioEngineDataStruct::GetAfterManFX(float* pOutput, const unsigned long frameCount)
   {
       if(!FXManualPannel.has_value()){
           return;
       }
       auto getres = getNowfPointer(frameCount);
       if(!getres.has_value()){
           return;
       }
       if(FXManualPannel->checkSomethingOn()){
           toFaustStylePCM(faustPcmPP, getres.value(), frameCount);
           FXManualPannel->addFX(faustPcmPP, frameCount);
           toLRStylePCM(faustPcmPP, pOutput, frameCount);
       }
       else{
   
           memcpy(pOutput, getres.value(), frameCount * CHANNEL * sizeof(float));
       }
   }
   
   void
   audioEngineDataStruct::Get(float* pOutput, unsigned long frameCount)
   {
       auto getres = getNowfPointer(frameCount);
       if(!getres.has_value()){
           return;
       }
       memcpy(pOutput, getres.value(), frameCount * CHANNEL * sizeof(float));
   }
   
   
   
   
   
   void
   FullPreRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
       auto rendered = reinterpret_cast<audioEngineDataStruct*>(pDevice->pUserData);
       rendered->Get(reinterpret_cast<float*>(pOutput), frameCount);
       rendered->CountUp(frameCount);
   }
   
   void
   HybridRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
       auto rendered = reinterpret_cast<audioEngineDataStruct*>(pDevice->pUserData);
       rendered->GetAfterManFX(reinterpret_cast<float*>(pOutput), frameCount);
       rendered->MusCtrPannel->GetPCMFrames(reinterpret_cast<float*>(pOutput), frameCount);
       rendered->CountUp(frameCount);
   }
   
   void
   FullManualRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
       auto Data = reinterpret_cast<audioEngineDataStruct*>(pDevice->pUserData);
       Data->MusCtrPannel->GetPCMFrames(reinterpret_cast<float*>(pOutput), frameCount);
       
       
   }
