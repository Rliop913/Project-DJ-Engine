
.. _program_listing_file_include_judge_PDJE_Judge.hpp:

Program Listing for File PDJE_Judge.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_PDJE_Judge.hpp>` (``include/judge/PDJE_Judge.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <optional>
   
   #include "PDJE_Core_DataLine.hpp"
   
   class PDJE_JUDGE{
   private:
       std::optional<PDJE_CORE_DATA_LINE> core;
   public:
       PDJE_JUDGE(const PDJE_CORE_DATA_LINE& core_line);
       ~PDJE_JUDGE() = default;
   };
