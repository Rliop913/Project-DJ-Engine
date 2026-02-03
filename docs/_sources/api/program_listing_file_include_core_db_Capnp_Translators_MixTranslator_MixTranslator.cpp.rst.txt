
.. _program_listing_file_include_core_db_Capnp_Translators_MixTranslator_MixTranslator.cpp:

Program Listing for File MixTranslator.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_Translators_MixTranslator_MixTranslator.cpp>` (``include/core/db/Capnp/Translators/MixTranslator/MixTranslator.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixTranslator.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   MixTranslator::MixTranslator()
   {
       usable_threads = std::thread::hardware_concurrency();
       if (usable_threads == 0) {
           usable_threads = 1;
       }
       mixs.emplace();
       bpms.emplace();
   }
   
   bool
   MixTranslator::Read(const CapReader<MixBinaryCapnpData> &binary)
   {
       if (!mixs->openMix(binary.Rp.value())) {
           critlog("failed to open mix data. from MixTranslator Read.");
           return false;
       }
       if (!bpms->getBpms(mixs.value())) {
           critlog("failed to get bpm datas. from MixTranslator Read");
           return false;
       }
       if (!mixs->WriteFrames(bpms.value())) {
           critlog("failed to write frames. from MixTranslator Read");
           return false;
       }
       return true;
       // if(!WriteFrames()){
       //     return false;
       // }
   }
   
   MixTranslator::~MixTranslator()
   {
   }
