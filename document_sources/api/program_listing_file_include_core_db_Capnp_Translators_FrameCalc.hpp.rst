
.. _program_listing_file_include_core_db_Capnp_Translators_FrameCalc.hpp:

Program Listing for File FrameCalc.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_Translators_FrameCalc.hpp>` (``include/core/db/Capnp/Translators/FrameCalc.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "FrameCalcCore.hpp"
   
   #include "CapnpBinary.hpp"
   
   // #include <hwy/highway.h>
   #include <hwy/aligned_allocator.h>
   #include <vector>
   
   // namespace hn = hwy::HWY_NAMESPACE;
   
   using SIMD_FLOAT = std::vector<float, hwy::AlignedAllocator<float>>;
   
   struct MixStruct {
       unsigned long long frame_in;
       unsigned long long frame_out;
       MBData::Reader RP;
   };
