
.. _program_listing_file_include_audioRender_MixMachine_MixMachine.cpp:

Program Listing for File MixMachine.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixMachine.cpp>` (``include/audioRender/MixMachine/MixMachine.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "MixMachine-inl.h"
   
   MixMachine::MixMachine()
   {
   }
   
   
   
   bool
   MixMachine::IDsort(const MixTranslator& tr)
   {
       try
       {
           for(auto i : tr.mixs.value().mixVec){
               long id = i.RP.getId();
   
               if(Memorized.find(id) == Memorized.end()){
                   Memorized[id] = std::vector<MixStruct>();
               }
               Memorized[id].push_back(i);
           }
   
           return true;
       }
       catch(std::exception& e)
       {
           critlog("failed to sort memorized datas. From MixMachine IDsort. logException: ");
           critlog(e.what());
           return false;
       }
   }
   
   
   HWY_EXPORT(INTEGRATE_PCM_SIMD);
   
   bool
   MixMachine::mix(litedb& db, const BPM& bpms)
   {
       auto num_threads = Memorized.size();
       std::vector<std::unique_ptr<std::thread>> renderPool;
       // renderPool.clear();
       renderPool.reserve(num_threads);
       for(auto& i: Memorized){
           renderPool.emplace_back(
               std::make_unique<std::thread>(
               [i, this, &db, &bpms](){
   
               auto MC = new MUSIC_CTR();
               auto DJ = new BattleDj();
               auto FX = new FaustEffects(SAMPLERATE);
   
               SIMD_FLOAT tempVec;
               DJ->GetDataFrom(*MC);
               for(auto j : i.second){
                   switch (j.RP.getType()){
   
                   case TypeEnum::BATTLE_DJ:
                       if(TypeWorks<TypeEnum::BATTLE_DJ>(j, *DJ)) break;
                       else continue;
   
                   case TypeEnum::LOAD:
                       if(TypeWorks<TypeEnum::LOAD>(j, *MC, db)
                           && TypeWorks<TypeEnum::LOAD>(j, *DJ)) break;
                       else continue;
   
                   case TypeEnum::CONTROL:
                       if(TypeWorks<TypeEnum::CONTROL>(j, *MC)) break;
                       else continue;
   
                   case TypeEnum::UNLOAD:
                       if(TypeWorks<TypeEnum::UNLOAD>(j, *MC)) break;
                       else continue;
   
                   case TypeEnum::EQ:
                       if(TypeWorks<TypeEnum::EQ>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::COMPRESSOR:
                       if(TypeWorks<TypeEnum::COMPRESSOR>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::FILTER:
                       if(TypeWorks<TypeEnum::FILTER>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::DISTORTION:
                       if(TypeWorks<TypeEnum::DISTORTION>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::ECHO:
                       if(TypeWorks<TypeEnum::ECHO>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::FLANGER:
                       if(TypeWorks<TypeEnum::FLANGER>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::OSC_FILTER:
                       if(TypeWorks<TypeEnum::OSC_FILTER>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::PANNER:
                       if(TypeWorks<TypeEnum::PANNER>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::PHASER:
                       if(TypeWorks<TypeEnum::PHASER>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::ROLL:
                       if(TypeWorks<TypeEnum::ROLL>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::TRANCE:
                       if(TypeWorks<TypeEnum::TRANCE>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::ROBOT:
                       if(TypeWorks<TypeEnum::ROBOT>(j, *FX, &tempVec)) break;
                       else continue;
   
                   case TypeEnum::VOL:
                       if(TypeWorks<TypeEnum::VOL>(j, *FX, &tempVec)) break;
                       else continue;
   
                   default:
                       break;
                   }
               }
   
               auto result = (*DJ) << MC->Execute(bpms, &tempVec, db);
               if(!result.has_value()){
                   FLAG_SOMETHING_WRONG_ID = i.first;
                   critlog("result has no value. From MixMachine mix. ErrID: ");
                   std::string logTemp = std::to_string(i.first);
                   critlog(logTemp);
                   return;
               }
               FX->consumeAll();
   
               HWY_DYNAMIC_DISPATCH(INTEGRATE_PCM_SIMD)(
                   tempVec,
                   renderLock,
                   rendered_out,
                   MC
               );
               delete MC;
               delete DJ;
               delete FX;
           
           }
           )
           );
       }
       for(auto& pool: renderPool){
           pool->join();
       }
       if(FLAG_SOMETHING_WRONG_ID != FLAG_ALL_IS_OK){
           critlog("mix failed because something is broken. From MixMachine mix");
           return false;
       }
       return true;
   }
   
   MixMachine::~MixMachine()
   {
   
   }
