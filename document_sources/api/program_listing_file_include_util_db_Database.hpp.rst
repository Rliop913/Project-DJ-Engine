
.. _program_listing_file_include_util_db_Database.hpp:

Program Listing for File Database.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_Database.hpp>` (``include\util\db\Database.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/db/keyvalue/Database.hpp"
   
   namespace PDJE_UTIL::db {
   
   template <keyvalue::KeyValueBackendConcept Backend>
   using Database = keyvalue::KeyValueDatabase<Backend>;
   
   } // namespace PDJE_UTIL::db
