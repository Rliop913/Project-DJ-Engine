
.. _program_listing_file_include_db_Capnp_Translators_MixTranslator_MixTranslator.hpp:

Program Listing for File MixTranslator.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_Capnp_Translators_MixTranslator_MixTranslator.hpp>` (``include/db/Capnp/Translators/MixTranslator/MixTranslator.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <vector>
   #include <algorithm>
   #include <string>
   #include <thread>
   #include <mutex>
   #include <optional>
   
   #include "CapnpBinary.hpp"
   #include "Mix.hpp"
   #include "Bpm.hpp"
   
   class MixTranslator{
   private:
       unsigned int usable_threads = 0;
   public:
       std::optional<MIX> mixs;
       std::optional<BPM> bpms;
   
       bool Read(const CapReader<MixBinaryCapnpData>& binary);
   
       MixTranslator();
       ~MixTranslator();
   };
