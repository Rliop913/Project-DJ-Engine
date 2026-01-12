
.. _program_listing_file_include_judge_PDJE_Judge.hpp:

Program Listing for File PDJE_Judge.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_PDJE_Judge.hpp>` (``include\judge\PDJE_Judge.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <cstdint>
   #include <optional>
   
   #include "Input_State.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Judge_Loop.hpp"
   #include <atomic>
   #include <thread>
   #include <unordered_map>
   #include <vector>
   namespace PDJE_JUDGE {
   enum JUDGE_STATUS {
       OK                   = 0,
       CORE_LINE_IS_MISSING = 1,
       INPUT_LINE_IS_MISSING,
       EVENT_RULE_IS_EMPTY,
       INPUT_RULE_IS_EMPTY,
       NOTE_OBJECT_IS_MISSING,
   };
   
   class PDJE_API JUDGE {
     private:
       std::optional<Judge_Loop> loop_obj;
   
     private:
       // thread relates
       std::optional<std::thread> loop;
   
     public:
       Judge_Init inits;
       JUDGE_STATUS
       Start();
       void
       End();
   
       JUDGE();
       ~JUDGE() = default;
   };
   }; // namespace PDJE_JUDGE
