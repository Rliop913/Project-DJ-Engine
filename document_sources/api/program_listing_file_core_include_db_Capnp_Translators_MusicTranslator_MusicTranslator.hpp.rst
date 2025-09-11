
.. _program_listing_file_core_include_db_Capnp_Translators_MusicTranslator_MusicTranslator.hpp:

Program Listing for File MusicTranslator.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_db_Capnp_Translators_MusicTranslator_MusicTranslator.hpp>` (``core_include\db\Capnp\Translators\MusicTranslator\MusicTranslator.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <optional>
   #include <vector>
   
   #include "CapnpBinary.hpp"
   #include "MusicBinary.capnp.h"
   
   #include "FrameCalc.hpp"
   
   #include "PDJE_EXPORT_SETTER.hpp"
   class PDJE_API MusicTranslator {
     public:
       BpmStruct bpms;
       bool
       Read(const CapReader<MusicBinaryCapnpData> &binary,
            unsigned long long                     startFrame);
   
       MusicTranslator()  = default;
       ~MusicTranslator() = default;
   };
