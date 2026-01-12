
.. _program_listing_file_include_core_db_Capnp_Translators_NoteTranslator_NoteTranslator.hpp:

Program Listing for File NoteTranslator.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_Translators_NoteTranslator_NoteTranslator.hpp>` (``include\core\db\Capnp\Translators\NoteTranslator\NoteTranslator.hpp``)

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
   #include "PDJE_OBJ_SETTER.hpp"
   #include <functional>
   
   class PDJE_API NoteTranslator {
     private:
       BpmStruct noteBpms;
   
     public:
       bool
       Read(const CapReader<NoteBinaryCapnpData> &binary,
            const BpmStruct                      &mainBpm,
            OBJ_SETTER_CALLBACK                  &lambdaCallback);
   
       NoteTranslator()  = default;
       ~NoteTranslator() = default;
   };
