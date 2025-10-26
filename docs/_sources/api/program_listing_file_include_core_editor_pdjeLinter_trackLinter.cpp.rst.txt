
.. _program_listing_file_include_core_editor_pdjeLinter_trackLinter.cpp:

Program Listing for File trackLinter.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_pdjeLinter_trackLinter.cpp>` (``include/core/editor/pdjeLinter/trackLinter.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "CapnpBinary.hpp"
   #include "MixBinary.capnp.h"
   #include "fileNameSanitizer.hpp"
   #include "pdjeLinter.hpp"
   #include <cstdint>
   #include <string>
   #include <unordered_map>
   
   using ID_LOADED = std::unordered_map<int32_t, int>;
   
   void
   FillIdHasLoad(ID_LOADED      &accumulate_data,
                 const TypeEnum &type,
                 const int32_t  &id)
   {
       if (!accumulate_data.contains(id)) {
           accumulate_data[id] = 0;
       }
   
       if (type == TypeEnum::LOAD) {
           accumulate_data[id] += 1;
       }
   }
   
   bool
   CheckIDHasLoad(const ID_LOADED &acc_data, UNSANITIZED &msg)
   {
       bool FLAG_OK = true;
       for (const auto &id : acc_data) {
           if (id.second != 1) {
               FLAG_OK = false;
               msg += " ID " + std::to_string(id.first) + " has " +
                      (id.second > 1
                           ? (std::to_string(id.second) + " load command.\n")
                           : "no load command.\n");
           }
       }
       return FLAG_OK;
   }
   
   template <>
   bool
   PDJE_Linter<trackdata>::Lint(const trackdata &target, UNSANITIZED &lint_msg)
   {
       auto trackReader = CapReader<MixBinaryCapnpData>();
       if (!trackReader.open(target.mixBinary)) {
           lint_msg = "failed to open mix data.";
           return false;
       }
       auto td = trackReader.Rp->getDatas();
   
       ID_LOADED id_has_load;
   
       for (size_t i = 0; i < td.size(); ++i) {
           FillIdHasLoad(id_has_load, td[i].getType(), td[i].getId());
       }
       bool FLAG_RESULT = true;
       if (!CheckIDHasLoad(id_has_load, lint_msg)) {
           FLAG_RESULT = false;
       }
   
       return FLAG_RESULT;
   }
