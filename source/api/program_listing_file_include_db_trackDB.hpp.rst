
.. _program_listing_file_include_db_trackDB.hpp:

Program Listing for File trackDB.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_trackDB.hpp>` (``include/db/trackDB.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <sqlite3.h>
   #include <string>
   
   #include "dbState.hpp"
   
   struct trackdata{
   public:
       std::string trackTitle;
       BIN mixBinary;
       BIN noteBinary;
       std::string cachedMixList;
   
       trackdata(stmt* dbstate);
   
       trackdata(
           const std::string& trackTitle__ = ""
       );
   
       bool GenSearchSTMT(stmt& dbstate, sqlite3* db);
   
       bool GenInsertSTMT(stmt& dbstate, sqlite3* db);
   
       bool GenEditSTMT(stmt& dbstate, sqlite3* db, trackdata& toEdit);
   
       bool GenDeleteSTMT(stmt& dbstate, sqlite3* db);
   };
