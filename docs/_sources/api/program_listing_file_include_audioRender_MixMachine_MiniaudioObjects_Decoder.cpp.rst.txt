
.. _program_listing_file_include_audioRender_MixMachine_MiniaudioObjects_Decoder.cpp:

Program Listing for File Decoder.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MiniaudioObjects_Decoder.cpp>` (``include/audioRender/MixMachine/MiniaudioObjects/Decoder.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Decoder.hpp"
   #include <filesystem>
   
   Decoder::Decoder()
   {
       ;
   }
   
   #include <iostream>
   bool 
   Decoder::init(const std::string& song_path, const std::string& root_path)
   {
       ma_decoder_config dconf = ma_decoder_config_init(ma_format_f32, CHANNEL, SAMPLERATE);
       namespace fs = std::filesystem;
       fs::path relative_path(song_path);
       fs::path root(root_path);
       fs::path fullpath = root.parent_path() / relative_path;
       fullpath = fullpath.lexically_normal();
       return ma_decoder_init_file(fullpath.c_str(), &dconf, &dec) == MA_SUCCESS;
   }   
   
   bool
   Decoder::changePos(FRAME_POS Pos)
   {
       return ma_decoder_seek_to_pcm_frame(&dec, Pos) == MA_SUCCESS;
   }
   
   bool
   Decoder::getPos(FRAME_POS& pos)
   {
       return ma_decoder_get_cursor_in_pcm_frames(&dec, &pos) == MA_SUCCESS;
   }
   
   bool
   Decoder::getRange(FRAME_POS numFrames, std::vector<float>& buffer)
   {
       FRAME_POS BufferSize = numFrames * CHANNEL;
       if(buffer.size() < BufferSize){
           buffer.resize(BufferSize);
       }
       if(ma_decoder_read_pcm_frames(&dec, buffer.data(), numFrames, NULL) != MA_SUCCESS){
           return false;
       }
       return true;
   }
   
   Decoder::~Decoder()
   {
       ma_decoder_uninit(&dec);
   }
