
.. _program_listing_file_core_include_db_Capnp_Translators_NoteTranslator_NoteTranslator.hpp:

Program Listing for File NoteTranslator.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_db_Capnp_Translators_NoteTranslator_NoteTranslator.hpp>` (``core_include\db\Capnp\Translators\NoteTranslator\NoteTranslator.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "CapnpBinary.hpp"
   #include "NoteBinary.capnp.h"
   #include <optional>
   #include <string>
   #include <vector>
   
   #include "FrameCalc.hpp"
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include <functional>
   
   using OBJ_SETTER_CALLBACK = std::function<void(
       const std::string,
       const std::string,
       const std::string,
       const std::string,
       const std::string,
       const unsigned long long,
       const unsigned long long)>;
   
   class PDJE_API NoteTranslator {
     public:
       BpmStruct noteBpms;
       bool
       Read(const CapReader<NoteBinaryCapnpData> &binary,
            const BpmStruct                      &mainBpm,
            OBJ_SETTER_CALLBACK                  &lambdaCallback);
   
       NoteTranslator()  = default;
       ~NoteTranslator() = default;
   };
