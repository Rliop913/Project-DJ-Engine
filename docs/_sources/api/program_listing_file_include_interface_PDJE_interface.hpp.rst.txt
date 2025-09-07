
.. _program_listing_file_include_interface_PDJE_interface.hpp:

Program Listing for File PDJE_interface.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_interface_PDJE_interface.hpp>` (``include/interface/PDJE_interface.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "NoteTranslator.hpp"
   #include "PDJE_Core_DataLine.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "audioPlayer.hpp"
   #include "dbRoot.hpp"
   #include "editorObject.hpp"
   enum PLAY_MODE { FULL_PRE_RENDER, HYBRID_RENDER, FULL_MANUAL_RENDER };
   class PDJE_API PDJE {
     private:
     public:
       std::shared_ptr<litedb> DBROOT;
       // std::optional<litedb> DBROOT;
       PDJE(const DONT_SANITIZE &rootDir);
   
       ~PDJE() = default;
   
       std::shared_ptr<audioPlayer>  player;
       std::shared_ptr<editorObject> editor;
       bool
       InitPlayer(PLAY_MODE          mode,
                  trackdata         &td,
                  const unsigned int FrameBufferSize);
   
       void
       ResetPlayer()
       {
           player.reset();
       }
   
       void
       CloseEditor()
       {
           editor.reset();
       }
   
       PDJE_CORE_DATA_LINE
       PullOutDataLine();
   
       bool
       InitEditor(const DONT_SANITIZE &auth_name,
                  const DONT_SANITIZE &auth_email,
                  const DONT_SANITIZE &projectRoot);
       bool
       GetNoteObjects(trackdata &td, OBJ_SETTER_CALLBACK &ObjectSetCallback);
       MUS_VEC
       SearchMusic(const UNSANITIZED &Title,
                   const UNSANITIZED &composer,
                   const double       bpm = -1);
       TRACK_VEC
       SearchTrack(const UNSANITIZED &Title);
       std::shared_ptr<audioPlayer>
       GetPlayerObject();
   
       std::shared_ptr<editorObject>
       GetEditorObject()
       {
           return editor;
       }
   };
   class PDJE_API ARGSETTER_WRAPPER {
     private:
       FXControlPannel *fxp;
   
     public:
       ARGSETTER_WRAPPER(FXControlPannel *pointer) : fxp(pointer) {};
       ~ARGSETTER_WRAPPER() = default;
       std::vector<std::string>
       GetFXArgKeys(FXList fx);
       void
       SetFXArg(FXList fx, const DONT_SANITIZE &key, double arg);
   };
