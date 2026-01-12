
.. _program_listing_file_include_core_db_Capnp_Translators_MixTranslator_MixTranslator.hpp:

Program Listing for File MixTranslator.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_Translators_MixTranslator_MixTranslator.hpp>` (``include\core\db\Capnp\Translators\MixTranslator\MixTranslator.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <algorithm>
   #include <mutex>
   #include <optional>
   #include <string>
   #include <thread>
   #include <vector>
   
   #include "Bpm.hpp"
   #include "CapnpBinary.hpp"
   #include "Mix.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   class PDJE_API MixTranslator {
     private:
       unsigned int usable_threads = 0;
   
     public:
       std::optional<MIX> mixs;
       std::optional<BPM> bpms;
   
       bool
       Read(const CapReader<MixBinaryCapnpData> &binary);
   
       MixTranslator();
       ~MixTranslator();
   };
