
.. _program_listing_file_include_core_editor_featureWrapper_JSONWrap_EditorArgs.hpp:

Program Listing for File EditorArgs.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_featureWrapper_JSONWrap_EditorArgs.hpp>` (``include/core/editor/featureWrapper/JSONWrap/EditorArgs.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "MixTranslator.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "fileNameSanitizer.hpp"
   #include <cstdint>
   
   struct PDJE_API MixArgs {
       TypeEnum        type      = TypeEnum::EQ;
       DetailEnum      details   = DetailEnum::HIGH;
       int             ID        = -1;
       SANITIZED_ORNOT first     = "";
       SANITIZED_ORNOT second    = "";
       SANITIZED_ORNOT third     = "";
       uint64_t       beat      = 0;
       uint64_t       subBeat   = 0;
       uint64_t       separate  = 0;
       uint64_t       Ebeat     = 0;
       uint64_t       EsubBeat  = 0;
       uint64_t       Eseparate = 0;
   };
   
   struct PDJE_API NoteArgs {
       SANITIZED_ORNOT Note_Type   = "";
       uint16_t        Note_Detail = 0;
       SANITIZED_ORNOT first       = "";
       SANITIZED_ORNOT second      = "";
       SANITIZED_ORNOT third       = "";
       uint64_t       beat        = 0;
       uint64_t       subBeat     = 0;
       uint64_t       separate    = 0;
       uint64_t       Ebeat       = 0;
       uint64_t       EsubBeat    = 0;
       uint64_t       Eseparate   = 0;
       uint64_t        railID      = 0;
   };
   
   struct PDJE_API MusicArgs {
       DONT_SANITIZE bpm      = "";
       uint64_t     beat     = -1;
       uint64_t     subBeat  = -1;
       uint64_t     separate = -1;
   };
   
   using MIX_W   = CapWriter<MixBinaryCapnpData>;
   using NOTE_W  = CapWriter<NoteBinaryCapnpData>;
   using MUSIC_W = CapWriter<MusicBinaryCapnpData>;
   
   using KEY       = DONT_SANITIZE;
   using KEY_VALUE = std::pair<DONT_SANITIZE, DONT_SANITIZE>;
   using KV_W      = std::vector<KEY_VALUE>;
