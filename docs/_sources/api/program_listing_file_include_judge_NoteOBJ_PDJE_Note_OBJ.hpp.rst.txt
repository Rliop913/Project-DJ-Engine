
.. _program_listing_file_include_judge_NoteOBJ_PDJE_Note_OBJ.hpp:

Program Listing for File PDJE_Note_OBJ.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_NoteOBJ_PDJE_Note_OBJ.hpp>` (``include/judge/NoteOBJ/PDJE_Note_OBJ.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Input_State.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include <cstdint>
   #include <iostream>
   #include <string>
   #include <unordered_map>
   #include <utility>
   #include <vector>
   namespace PDJE_JUDGE {
   constexpr int BUFFER_MAIN = 0;
   constexpr int BUFFER_SUB  = 1;
   
   using GLOBAL_TIME = uint64_t;
   using LOCAL_TIME  = uint64_t;
   struct PDJE_API NOTE {
       std::string type;
       uint16_t    detail;
       std::string first;
       std::string second;
       std::string third;
       LOCAL_TIME  microsecond = 0;
       bool        used        = false;
       bool        isDown      = true;
   };
   
   using NOTE_VEC   = std::vector<NOTE>;
   using P_NOTE_VEC = std::vector<NOTE *>;
   
   struct PDJE_API NOTE_ITR {
       NOTE_VEC           vec;
       NOTE_VEC::iterator itr;
   };
   
   using DEVID_TO_NOTE = std::unordered_map<uint64_t, NOTE_ITR>;
   
   class PDJE_API OBJ {
     private:
       DEVID_TO_NOTE Buffer_Main;
       DEVID_TO_NOTE Buffer_Sub;
   
       template <int I>
       DEVID_TO_NOTE *
       pick_dan()
       {
           if constexpr (I == BUFFER_MAIN) {
               return &Buffer_Main;
           } else {
               return &Buffer_Sub;
           }
       }
   
     public:
       void
       Sort(); // use only for init
   
       template <int I>
       void
       Fill(const NOTE &data, uint64_t rail_id)
       {
           static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                         "invalid use of fill.");
           DEVID_TO_NOTE *dan = pick_dan<I>();
           (*dan)[rail_id].vec.push_back(data);
       }
   
       template <int I>
       void
       Get(const LOCAL_TIME limit, uint64_t railID, P_NOTE_VEC &found)
       {
           static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                         "invalid use of get.");
           DEVID_TO_NOTE *dan = pick_dan<I>();
   
           found.clear();
           auto &note = (*dan)[railID];
           if (note.vec.empty()) {
               return;
           }
           while (true) { // pull iterator
               if (note.itr != note.vec.end() && note.itr->used) {
   
                   ++note.itr;
               } else {
                   break;
               }
           }
   
           auto titr = note.itr;
   
           while (true) {
               if ((titr != note.vec.end()) && titr->microsecond <= limit &&
                   !titr->used) {
   
                   found.push_back(std::addressof(*titr));
                   ++titr;
               } else {
                   break;
               }
           }
       }
   
       template <int I>
       void
       Cut(const LOCAL_TIME limit, std::unordered_map<uint64_t, NOTE_VEC> &cuts)
       {
           static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                         "invalid use of cut.");
           DEVID_TO_NOTE *dan = pick_dan<I>();
   
           for (auto &rail : *dan) {
               if (rail.second.vec.empty()) {
                   continue;
               }
               auto titr = rail.second.itr;
   
               while (titr != rail.second.vec.end() &&
                      titr->microsecond <= limit) {
                   if (!titr->used) {
                       cuts[rail.first].push_back(*titr);
                       titr->used = true;
                   }
                   ++titr;
               }
           }
       }
       OBJ()  = default;
       ~OBJ() = default;
   };
   }; // namespace PDJE_JUDGE
