
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_MUSCTR.cpp:

Program Listing for File Type_MUSCTR.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_MUSCTR.cpp>` (``include/audioRender/MixMachine/MixTypes/Type_MUSCTR.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::LOAD, MUSIC_CTR>
   (MixStruct& ms, MUSIC_CTR& data, litedb& db)
   {
       if(!data.setLOAD(ms.RP, db, ms.frame_in)){
           return false;
       }
       return true;
   }
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::LOAD, BattleDj>
   (MixStruct& ms, BattleDj& data)
   {
       data.StartPos = ms.frame_in;
       return true;
   }
   
   // template<>
   // bool
   // MixMachine::TypeWorks<TypeEnum::LOAD, FilterFAUST>
   // (MixStruct& ms, FilterFAUST& data)
   // {
   //     data.StartPos = ms.frame_in;
   //     return true;
   // }
   
   
   
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::BATTLE_DJ, BattleDj>
   (MixStruct& ms, BattleDj& data)
   {
       switch (ms.RP.getDetails()){
       case DetailEnum::SPIN:
           if(!data.Spin(ms)){
               return false;
           }
           break;
       case DetailEnum::REV:
           if(!data.Rev(ms)){
               return false;
           }
           break;
       case DetailEnum::PITCH:
           if(!data.Pitch(ms)){
               return false;
           }
           break;
       case DetailEnum::SCRATCH:
           if(!data.Scratch(ms)){
               return false;
           }
           break;
       default:
           return false;
       }
       return true;
   }
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::CONTROL>
   (MixStruct& ms, MUSIC_CTR& data)
   {
       switch (ms.RP.getDetails())
       {
       case DetailEnum::PAUSE:
           {
               PlayPosition pause;
               pause.Gidx = ms.frame_in;
               pause.Lidx = 0;
               pause.status = MIXSTATE::PAUSE;
               data.QDatas.pos.push_back(pause);
           }
           break;
       case DetailEnum::CUE:
           try
           {
               PlayPosition cuepos;
               cuepos.Gidx = ms.frame_in;
               cuepos.Lidx = std::stoull(ms.RP.getFirst().cStr());
               cuepos.status = MIXSTATE::PLAY;
               data.QDatas.pos.push_back(cuepos);
           }
           catch(...)
           {
               return false;
           }
           break;
       default:
           break;
       }
       return true;
   }
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::UNLOAD>
   (MixStruct& ms, MUSIC_CTR& data)
   {
       PlayPosition unload;
       unload.Gidx = ms.frame_in;
       unload.status = MIXSTATE::END;
       data.QDatas.pos.push_back(unload);
       return true;
   }
   
