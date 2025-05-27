
.. _program_listing_file_include_MainObjects_tempDBObject_tempDB.cpp:

Program Listing for File tempDB.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_tempDBObject_tempDB.cpp>` (``include/MainObjects/tempDBObject/tempDB.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "tempDB.hpp"
   
   bool
   tempDB::Open(fs::path& projectRoot)
   {
       return 
       tempROOT->openDB(
           (projectRoot / "LOCALDB.pdjedb").string()
       );
   }
   
   bool
   tempDB::BuildProject(trackdata& td, std::vector<musdata>& mds)
   {
       auto dbposition = tempROOT->getRoot();
       tempROOT.reset();
       if(!fs::remove(dbposition)){
           return false;
       }
       tempROOT.emplace();
   
       if(!tempROOT->openDB(dbposition))   return false;
       if(!(tempROOT.value() <= td))       return false;
       for(auto& i : mds){
           if(!(tempROOT.value() <= i))      return false;
       }
       return true;
   }
