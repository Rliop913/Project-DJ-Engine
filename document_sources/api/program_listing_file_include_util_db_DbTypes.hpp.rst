
.. _program_listing_file_include_util_db_DbTypes.hpp:

Program Listing for File DbTypes.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_DbTypes.hpp>` (``include\util\db\DbTypes.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <cstddef>
   #include <string>
   #include <vector>
   
   namespace PDJE_UTIL::db {
   
   using Key   = std::string;
   using Text  = std::string;
   using Bytes = std::vector<std::byte>;
   
   struct OpenOptions {
       bool create_if_missing  = false;
       bool truncate_if_exists = false;
       bool read_only          = false;
   };
   
   } // namespace PDJE_UTIL::db
