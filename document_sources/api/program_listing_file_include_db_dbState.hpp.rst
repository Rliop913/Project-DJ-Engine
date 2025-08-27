
.. _program_listing_file_include_db_dbState.hpp:

Program Listing for File dbState.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_dbState.hpp>` (``include\db\dbState.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <string>
   #include <vector>
   
   #include <sqlite3.h>
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "fileNameSanitizer.hpp"
   #ifdef WIN32
       using u_int8_t = uint8_t;
   #endif
   
   using BIN = std::vector<u_int8_t>;
   
   enum COL_TYPE{
       PDJE_INT,
       PDJE_DOUBLE,
       PDJE_TEXT,
       PDJE_BLOB
   };
   
   struct PDJE_API stmt
   {
   public:
   
       std::string placeHold;
   
       sqlite3_stmt* S = nullptr;
       int bind_null(int idx);
       int bind_text(int idx, SANITIZED_ORNOT& str);
   
   
       // int bind_u8text(int idx, std::u8string& str);
       int bind_blob(int idx, BIN& bin);
       int bind_double(int idx, double num);
       int bind_int(int idx, double num);
       
       template<int T_COL, typename res>
       res
       colGet(int idx);
   
       bool activate(sqlite3* db);
   
       stmt();
       ~stmt();
   };
