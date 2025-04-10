
.. _program_listing_file_include_db_Capnp_Translators_MixTranslator_Mix.hpp:

Program Listing for File Mix.hpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_Capnp_Translators_MixTranslator_Mix.hpp>` (``include/db/Capnp/Translators/MixTranslator/Mix.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <algorithm>
   #include <thread>
   #include <vector>
   #include <mutex>
   
   #include "FrameCalc.hpp"
   
   class BPM;
   
   class MIX{
   private:
       unsigned int usable_threads;
   public:
       std::vector<MixStruct> mixVec;
       bool openMix(const MixBinaryCapnpData::Reader& Rptr);
       bool WriteFrames(BPM& bpmm);
   
       MIX();
       ~MIX();
   };
