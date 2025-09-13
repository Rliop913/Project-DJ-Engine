
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
   
   struct PDJE_API MixArgs {
       TypeEnum        type      = TypeEnum::EQ;
       DetailEnum      details   = DetailEnum::HIGH;
       int             ID        = -1;
       SANITIZED_ORNOT first     = "";
       SANITIZED_ORNOT second    = "";
       SANITIZED_ORNOT third     = "";
       long long       beat      = -1;
       long long       subBeat   = -1;
       long long       separate  = -1;
       long long       Ebeat     = -1;
       long long       EsubBeat  = -1;
       long long       Eseparate = -1;
   };
   
   struct PDJE_API NoteArgs {
       SANITIZED_ORNOT Note_Type   = "";
       SANITIZED_ORNOT Note_Detail = "";
       SANITIZED_ORNOT first       = "";
       SANITIZED_ORNOT second      = "";
       SANITIZED_ORNOT third       = "";
       long long       beat        = -1;
       long long       subBeat     = -1;
       long long       separate    = -1;
       long long       Ebeat       = -1;
       long long       EsubBeat    = -1;
       long long       Eseparate   = -1;
   };
   
   struct PDJE_API MusicArgs {
       DONT_SANITIZE bpm      = "";
       long long     beat     = -1;
       long long     subBeat  = -1;
       long long     separate = -1;
   };
   
   using MIX_W   = CapWriter<MixBinaryCapnpData>;
   using NOTE_W  = CapWriter<NoteBinaryCapnpData>;
   using MUSIC_W = CapWriter<MusicBinaryCapnpData>;
   
   using KEY       = DONT_SANITIZE;
   using KEY_VALUE = std::pair<DONT_SANITIZE, DONT_SANITIZE>;
   using KV_W      = std::vector<KEY_VALUE>;
