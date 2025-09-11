
.. _program_listing_file_core_include_interface_PDJE_polyglot_wraps.cpp:

Program Listing for File PDJE_polyglot_wraps.cpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_interface_PDJE_polyglot_wraps.cpp>` (``core_include\interface\PDJE_polyglot_wraps.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_LOG_SETTER.hpp"
   #include "PDJE_interface.hpp"
   
   std::shared_ptr<audioPlayer>
   PDJE::GetPlayerObject()
   {
       return player;
   }
   
   std::vector<DONT_SANITIZE>
   ARGSETTER_WRAPPER::GetFXArgKeys(FXList fx)
   {
       if (fxp == nullptr) {
           warnlog("cannot use wrapper. fx pointer is nullptr. from "
                   "ARGSETTER_WRAPPER GetFXArgKeys");
           return std::vector<DONT_SANITIZE>();
       }
       auto                       argkey = fxp->GetArgSetter(fx);
       std::vector<DONT_SANITIZE> keylist;
       for (auto &i : argkey) {
           keylist.push_back(i.first);
       }
       return keylist;
   }
   
   void
   ARGSETTER_WRAPPER::SetFXArg(FXList fx, const DONT_SANITIZE &key, double arg)
   {
       if (fxp == nullptr) {
           warnlog("cannot use wrapper. fx pointer is nullptr. from "
                   "ARGSETTER_WRAPPER SetFXArg");
           return;
       }
       auto argsetter = fxp->GetArgSetter(fx);
       argsetter[key](arg);
       return;
   }
