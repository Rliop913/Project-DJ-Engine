
.. _program_listing_file_include_core_audioRender_audioRender.cpp:

Program Listing for File audioRender.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_audioRender.cpp>` (``include/core/audioRender/audioRender.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "audioRender.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   bool
   audioRender::LoadTrack(litedb &db, trackdata &td)
   {
       auto searchRes = db << td;
       if (!searchRes.has_value()) {
           critlog("failed to search track from database because something wrong. "
                   "From audioRender LoadTrack");
           return false;
       }
       if (searchRes->empty()) {
           warnlog(
               "track data not found from database. From audioRender LoadTrack");
           return false;
       }
       auto mb = CapReader<MixBinaryCapnpData>();
       if (!mb.open(searchRes.value()[0].mixBinary)) {
           critlog("failed to open capnpMusicBinary. From audioRender LoadTrack");
           return false;
       }
       auto mt = MixTranslator();
       if (!(mt.Read(mb))) {
           critlog("failed to read capnpmixBinary. From audioRender LoadTrack");
           return false;
       }
       auto mm = MixMachine();
   
       if (!mm.IDsort(mt)) {
           critlog("failed to sort idDatas. From audioRender LoadTrack");
           return false;
       }
   
       if (!mm.mix(db, mt.bpms.value())) {
           critlog("failed to mix datas. From audioRender LoadTrack");
           return false;
       }
       rendered_frames = std::move(mm.rendered_out);
   
       if (!rendered_frames.has_value()) {
           critlog(
               "failed to mix. no frames returned. From audioRender LoadTrack");
           return false;
       }
       return true;
   }
   
   bool
   audioRender::LoadTrackFromMixData(litedb &db, BIN &mixData)
   {
   
       auto mb = CapReader<MixBinaryCapnpData>();
       if (!mb.open(mixData)) {
           critlog(
               "failed to open mixData. From audioRender LoadTrackFromMixData");
           return false;
       }
       auto mt = MixTranslator();
       if (!(mt.Read(mb))) {
           critlog("failed to read capnpmixBinaryData. From audioRender "
                   "LoadTrackFromMixData");
           return false;
       }
       auto mm = MixMachine();
   
       if (!mm.IDsort(mt)) {
           critlog(
               "failed to sort IDDatas. From audioRender LoadTrackFromMixData");
           return false;
       }
   
       if (!mm.mix(db, mt.bpms.value())) {
           critlog("failed to mix datas. From audioRender LoadTrackFromMixData");
           return false;
       }
       rendered_frames = std::move(mm.rendered_out);
   
       if (!rendered_frames.has_value()) {
           critlog("failed to mix. no frames returned. From audioRender "
                   "LoadTrackFromMixData");
           return false;
       }
       return true;
   }
