
.. _program_listing_file_include_db_Capnp_Translators_MixTranslator_Bpm.hpp:

Program Listing for File Bpm.hpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_Capnp_Translators_MixTranslator_Bpm.hpp>` (``include/db/Capnp/Translators/MixTranslator/Bpm.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <mutex>
   #include <string>
   #include <thread>
   #include <vector>
   
   #include "FrameCalc.hpp"
   #include "Mix.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   class PDJE_API BPM {
     private:
       unsigned usable_threads;
   
     public:
       BpmStruct bpmVec;
       bool
       getBpms(MIX &mixx);
       BPM();
       ~BPM();
   };
