
.. _program_listing_file_include_audioRender_audioRender.cpp:

Program Listing for File audioRender.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_audioRender.cpp>` (``include/audioRender/audioRender.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "audioRender.hpp"
   
   bool
   audioRender::LoadTrack(litedb& db, trackdata& td)
   {
       auto searchRes = db << td;
       if(!searchRes.has_value()){
           return false;
       }
       auto mb = CapReader<MixBinaryCapnpData>();
       if(!mb.open(searchRes.value()[0].mixBinary)){
           return false;
       }
       auto mt = MixTranslator();
       if(!(mt.Read(mb))){
           return false;
       }
       auto mm = MixMachine();
   
       if(!mm.IDsort(mt)){
           return false;
       }
       
       if(!mm.mix(db, mt.bpms.value())){
           return false;
       }
       rendered_frames = std::move(mm.rendered_out);
       
       if(!rendered_frames.has_value()){
           return false;
       }
       return true;
   }
   
   bool
   audioRender::LoadTrackFromMixData(litedb& db, BIN& mixData)
   {
   
       auto mb = CapReader<MixBinaryCapnpData>();
       if(!mb.open(mixData)){
           return false;
       }
       auto mt = MixTranslator();
       if(!(mt.Read(mb))){
           return false;
       }
       auto mm = MixMachine();
   
       if(!mm.IDsort(mt)){
           return false;
       }
       
       if(!mm.mix(db, mt.bpms.value())){
           return false;
       }
       rendered_frames = std::move(mm.rendered_out);
       
       if(!rendered_frames.has_value()){
           return false;
       }
       return true;
   }
