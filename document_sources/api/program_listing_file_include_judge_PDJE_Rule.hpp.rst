
.. _program_listing_file_include_judge_PDJE_Rule.hpp:

Program Listing for File PDJE_Rule.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_PDJE_Rule.hpp>` (``include/judge/PDJE_Rule.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Input_State.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include <cstddef>
   #include <cstdint>
   #include <functional>
   #include <string>
   namespace PDJE_JUDGE {
   
   enum DEVICE_MOUSE_EVENT {
       BTN_EX   = 0,
       BTN_SIDE = 1,
       BTN_M,
       BTN_R,
       BTN_L,
       WHEEL_X,
       WHEEL_Y,
       AXIS_MOVE
   };
   
   struct PDJE_API INPUT_RULE {
       std::string   Device_ID  = "";
       PDJE_Dev_Type DeviceType = PDJE_Dev_Type::UNKNOWN;
       BITMASK       DeviceKey  = 0;
       bool
       operator==(const INPUT_RULE &) const = default;
   };
   
   struct PDJE_API INPUT_SETTING {
       uint64_t MatchRail          = 0;
       int64_t  offset_microsecond = 0;
   };
   
   struct PDJE_API INPUT_CONFIG : INPUT_RULE, INPUT_SETTING {
       // uint64_t MatchRail          = 0;
       // int64_t  offset_microsecond = 0;
   };
   
   struct PDJE_API EVENT_RULE {
       uint64_t miss_range_microsecond = 0;
       uint64_t use_range_microsecond  = 0;
   };
   
   }; // namespace PDJE_JUDGE
   
   template <> struct std::hash<PDJE_JUDGE::INPUT_RULE> {
       std::size_t
       operator()(const PDJE_JUDGE::INPUT_RULE &rule) const noexcept
       {
           size_t h1 = std::hash<int>()(static_cast<int>(rule.DeviceType));
           size_t h2 = std::hash<BITMASK>()(rule.DeviceKey);
           size_t h3 = std::hash<std::string>()(rule.Device_ID);
   
           size_t seed = h1;
           seed ^= h2 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 3);
           seed ^= h3 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 3);
           return seed;
       }
   };
