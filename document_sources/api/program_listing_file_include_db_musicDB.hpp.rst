
.. _program_listing_file_include_db_musicDB.hpp:

Program Listing for File musicDB.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_musicDB.hpp>` (``include\db\musicDB.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <string>
   #include <vector>
   #include <sqlite3.h>
   
   #include "dbState.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "fileNameSanitizer.hpp"
   
   struct PDJE_API musdata{
   public:
       SANITIZED title;
       SANITIZED composer;
       SANITIZED_ORNOT musicPath;
       BIN bpmBinary;
       double bpm = -1.0;
       DONT_SANITIZE firstBar;
       musdata(stmt* dbstate);
   
       musdata(
           const UNSANITIZED& title__ = "",
           const UNSANITIZED& composer__ = "",
           const SANITIZED_ORNOT& musicPath__ = "",
           const double bpm__ = -1.0
       );
       bool GenSearchSTMT(stmt& dbstate, sqlite3* db);
   
       bool GenInsertSTMT(stmt& dbstate, sqlite3* db);
   
       bool GenEditSTMT(stmt& dbstate, sqlite3* db, musdata& toEdit);
   
       bool GenDeleteSTMT(stmt& dbstate, sqlite3* db);
   
   };
   
   
