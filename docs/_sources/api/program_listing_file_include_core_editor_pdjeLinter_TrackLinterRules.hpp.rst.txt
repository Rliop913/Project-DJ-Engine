
.. _program_listing_file_include_core_editor_pdjeLinter_TrackLinterRules.hpp:

Program Listing for File TrackLinterRules.hpp
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_pdjeLinter_TrackLinterRules.hpp>` (``include\core\editor\pdjeLinter\TrackLinterRules.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "fileNameSanitizer.hpp"
   
   #include <cstdint>
   #include <string>
   #include <unordered_map>
   
   namespace PDJE_TRACK_LINTER_RULES {
   
   using TrackLinterIdCount = std::unordered_map<int32_t, int>;
   
   inline void
   AccumulateIf(TrackLinterIdCount &counts, const int32_t id, const bool should_count)
   {
       if (!counts.contains(id)) {
           counts[id] = 0;
       }
       if (should_count) {
           counts[id] += 1;
       }
   }
   
   inline bool
   ValidateExactlyOne(const TrackLinterIdCount &counts,
                      const char               *label,
                      UNSANITIZED              &msg)
   {
       bool ok = true;
       for (const auto &entry : counts) {
           if (entry.second != 1) {
               ok = false;
               msg += " ID " + std::to_string(entry.first) + " has ";
               if (entry.second > 1) {
                   msg += std::to_string(entry.second) + " " + label +
                          " command. render failed.\n";
               } else {
                   msg += "no " + std::string(label) + " command. render failed.\n";
               }
           }
       }
       return ok;
   }
   
   } // namespace PDJE_TRACK_LINTER_RULES
