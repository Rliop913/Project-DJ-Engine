
.. _program_listing_file_include_audioRender_ManualMix_MusicControlPannel.hpp:

Program Listing for File MusicControlPannel.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_MusicControlPannel.hpp>` (``include\audioRender\ManualMix\MusicControlPannel.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Decoder.hpp"
   
   #include <map>
   
   #include "ManualMix.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "dbRoot.hpp"
   #include <filesystem>
   #include "fileNameSanitizer.hpp"
   
   namespace fs = std::filesystem;
   // #undef HWY_TARGET_INCLUDE
   // #define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
   // #include "hwy/foreach_target.h"
   // #include <hwy/highway.h>
   
   
   using LOADED_LIST   = std::vector<std::string>;
   
   
   
   
   
   struct MusicOnDeck{
       bool play = false;
       Decoder dec;
       FXControlPannel* fxP;
       MusicOnDeck() : fxP(new FXControlPannel(48000)) {};
       ~MusicOnDeck(){
           // ma_decoder_uninit(&dec);
           delete fxP;
       }
   
   };
   
   
   
   using LOADS         = std::map<std::string, MusicOnDeck>;
   
   class PDJE_API MusicControlPannel{
   private:
   
       LOADS deck; 
       unsigned long fsize;
       std::vector<float> L;
       std::vector<float> R;
       float* FaustStyle[2];
       SIMD_FLOAT tempFrames;
   
   public:
       bool LoadMusic(litedb& ROOTDB, const musdata& Mus);
   
       bool CueMusic(const UNSANITIZED& title, const unsigned long long newPos);
   
       bool SetMusic(const UNSANITIZED& title, const bool onOff);
   
       LOADED_LIST GetLoadedMusicList();
   
       bool UnloadMusic(const UNSANITIZED& title);
   
       bool GetPCMFrames(float* array, const unsigned long FrameSize);
       
       FXControlPannel* getFXHandle(const UNSANITIZED& title);
       MusicControlPannel(const unsigned long FrameSize): fsize(FrameSize){}
       ~MusicControlPannel();
   
   };
   
