
.. _program_listing_file_include_interface_PDJE_polyglot_wraps.cpp:

Program Listing for File PDJE_polyglot_wraps.cpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_interface_PDJE_polyglot_wraps.cpp>` (``include/interface/PDJE_polyglot_wraps.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_interface.hpp"
   
   
   audioPlayer*
   PDJE::GetPlayerObject()
   {
       if(player.has_value()){
           return &player.value();
       }
       else{
           return nullptr;
       }
   }
   
   std::vector<std::string>
   ARGSETTER_WRAPPER::GetFXArgKeys(FXList fx)
   {
       if(fxp == nullptr){
           return std::vector<std::string>();
       }
       auto argkey = fxp->GetArgSetter(fx);
       std::vector<std::string> keylist;
       for(auto& i : argkey){
           keylist.push_back(i.first);
       }
       return keylist;
   }
   
   void
   ARGSETTER_WRAPPER::SetFXArg(
       FXList fx, 
       const std::string& key, 
       double arg
       )
   {
       if(fxp == nullptr){
           return;
       }
       auto argsetter = fxp->GetArgSetter(fx);
       argsetter[key](arg);
       return;
   }
