
.. _program_listing_file_include_judge_InputParser_InputParser.hpp:

Program Listing for File InputParser.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_InputParser_InputParser.hpp>` (``include/judge/InputParser/InputParser.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include "PDJE_Rule.hpp"
   #include <cstdint>
   #include <unordered_map>
   #include <vector>
   
   namespace PDJE_JUDGE {
   
   using INPUT_RAW = std::pair<PDJE_Input_Log *, uint64_t>;
   using DEV_ID    = std::string;
   struct PARSE_OUT {
       std::vector<PDJE_Input_Log> logs;
       uint64_t                    lowest;
       uint64_t                    highest;
   };
   
   class InputParser {
     private:
       PARSE_OUT outCache;
   
     public:
       std::unordered_map<RAIL_META, RAIL_SETTINGS> railData;
       std::unordered_map<DEV_ID, OFFSET>           offsetData;
       PARSE_OUT *
       Parse(const INPUT_RAW &raw);
       InputParser() = default;
   
       ~InputParser() = default;
   };
   } // namespace PDJE_JUDGE
