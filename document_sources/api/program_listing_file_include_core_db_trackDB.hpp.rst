
.. _program_listing_file_include_core_db_trackDB.hpp:

Program Listing for File trackDB.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_trackDB.hpp>` (``include\core\db\trackDB.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <sqlite3.h>
   #include <string>
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "dbState.hpp"
   #include "fileNameSanitizer.hpp"
   struct PDJE_API trackdata {
     public:
       SANITIZED trackTitle;
       BIN       mixBinary;
       BIN       noteBinary;
       SANITIZED cachedMixList;
   
       trackdata(stmt *dbstate);
   
       trackdata(const UNSANITIZED &trackTitle__ = "");
   
       bool
       GenSearchSTMT(stmt &dbstate, sqlite3 *db);
   
       bool
       GenInsertSTMT(stmt &dbstate, sqlite3 *db);
   
       bool
       GenEditSTMT(stmt &dbstate, sqlite3 *db, trackdata &toEdit);
   
       bool
       GenDeleteSTMT(stmt &dbstate, sqlite3 *db);
   };
