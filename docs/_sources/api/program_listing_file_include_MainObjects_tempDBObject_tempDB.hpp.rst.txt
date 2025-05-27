
.. _program_listing_file_include_MainObjects_tempDBObject_tempDB.hpp:

Program Listing for File tempDB.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_tempDBObject_tempDB.hpp>` (``include/MainObjects/tempDBObject/tempDB.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <filesystem>
   namespace fs = std::filesystem;
   
   #include "dbRoot.hpp"
   
   class tempDB{
   private:
       std::optional<litedb> tempROOT;
   public:
       bool Open(fs::path& projectRoot);
       bool BuildProject(trackdata& td, std::vector<musdata>& mds);
       litedb& GetBuildedProject(){
           return tempROOT.value();
       }
       tempDB() = default;
       ~tempDB() = default;
       
   };
