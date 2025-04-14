
.. _program_listing_file_include_audioRender_ManualMix_MusicControlPannel.hpp:

Program Listing for File MusicControlPannel.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_MusicControlPannel.hpp>` (``include/audioRender/ManualMix/MusicControlPannel.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <miniaudio.h>
   
   #include <map>
   
   #include "ManualMix.hpp"
   
   
   
   using TITLE         = std::string;
   
   using LOADED_LIST   = std::vector<TITLE>;
   
   struct MusicOnDeck{
       bool play = false;
       ma_decoder dec;
       FXControlPannel* fxP;
       MusicOnDeck() : fxP(new FXControlPannel(48000)) {};
       ~MusicOnDeck(){
           ma_decoder_uninit(&dec);
           delete fxP;
       }
   
   };
   
   
   using LOADS         = std::map<TITLE, MusicOnDeck>;
   
   
   class MusicControlPannel{
   private:
   
       LOADS deck; 
       unsigned long fsize;
       std::vector<float> L;
       std::vector<float> R;
       float* FaustStyle[2];
       SIMD_FLOAT tempFrames;
   
   public:
       int LoadMusic(const musdata& Mus);
   
       bool CueMusic(const TITLE& title, const unsigned long long newPos);
   
       bool SetMusic(const TITLE& title, const bool onOff);
   
       LOADED_LIST GetLoadedMusicList();
   
       bool UnloadMusic(const TITLE& title);
   
       bool GetPCMFrames(float* array, const unsigned long FrameSize);
   
       FXControlPannel* getFXHandle(const TITLE& title);
       MusicControlPannel(const unsigned long FrameSize): fsize(FrameSize){}
       ~MusicControlPannel();
   
   };
