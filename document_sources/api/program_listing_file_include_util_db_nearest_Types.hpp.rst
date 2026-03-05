
.. _program_listing_file_include_util_db_nearest_Types.hpp:

Program Listing for File Types.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_nearest_Types.hpp>` (``include\util\db\nearest\Types.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/db/DbTypes.hpp"
   
   #include <optional>
   #include <vector>
   
   namespace PDJE_UTIL::db::nearest {
   
   using Embedding = std::vector<float>;
   
   struct Item {
       Key                 id {};
       Embedding           embedding {};
       std::optional<Text> text_payload {};
       std::optional<Bytes> bytes_payload {};
   };
   
   struct SearchOptions {
       std::size_t limit    = 10;
       int         search_k = -1;
   };
   
   struct SearchHit {
       Key                  id {};
       float                distance = 0.0F;
       std::optional<Text>  text_payload {};
       std::optional<Bytes> bytes_payload {};
   };
   
   } // namespace PDJE_UTIL::db::nearest
