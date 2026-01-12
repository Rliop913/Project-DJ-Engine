
.. _program_listing_file_include_core_MainObjects_tempDBObject_tempDB.cpp:

Program Listing for File tempDB.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_tempDBObject_tempDB.cpp>` (``include\core\MainObjects\tempDBObject\tempDB.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "tempDB.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   bool
   tempDB::Open(const fs::path &projectRoot)
   {
       if (tempROOT.has_value())
           tempROOT.reset();
   
       tempROOT.emplace();
       bool openRes = tempROOT->openDB((projectRoot / fs::path("LOCALDB")));
       if (!openRes) {
           critlog("failed to open local database. from tempDB Open. path: ");
           fs::path logPath = (projectRoot / fs::path("LOCALDB"));
           critlog(logPath.generic_string());
       }
       return openRes;
   }
   
   bool
   tempDB::BuildProject(trackdata &td, std::vector<musdata> &mds)
   {
       auto dbposition = tempROOT->getRoot();
       tempROOT.reset();
       try {
   
           if (!fs::remove_all(dbposition)) {
               critlog("failed to remove local database. from tempDB "
                       "BuildProject. path: ");
               critlog(dbposition.generic_string());
               return false;
           }
       } catch (std::exception &e) {
           critlog("failed to remove local database. from tempDB BuildProject. "
                   "ErrException: ");
           critlog(e.what());
           return false;
       }
       tempROOT.emplace();
   
       if (!tempROOT->openDB(dbposition)) {
           critlog(
               "failed to open local database. from tempDB BuildProject. path: ");
           critlog(dbposition.generic_string());
           return false;
       }
       if (!(tempROOT.value() <= td)) {
           critlog("failed to push trackdata to local database. from tempDB "
                   "BuildProject. trackTitle: ");
           critlog(td.trackTitle);
           return false;
       }
       for (auto &i : mds) {
           if (!(tempROOT.value() <= i)) {
               critlog("failed to push musicdata to local database. from tempDB "
                       "BuildProject. musicTitle: ");
               critlog(i.title);
               return false;
           }
       }
       return true;
   }
