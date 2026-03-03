
.. _program_listing_file_include_util_db_nearest_BackendConcept.hpp:

Program Listing for File BackendConcept.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_nearest_BackendConcept.hpp>` (``include/util/db/nearest/BackendConcept.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/Result.hpp"
   #include "util/db/nearest/Types.hpp"
   
   #include <concepts>
   #include <span>
   #include <string_view>
   #include <vector>
   
   namespace PDJE_UTIL::db::nearest {
   
   template <class Backend>
   concept NearestNeighborBackendConcept =
       requires(typename Backend::config_type cfg,
                Backend                      backend,
                std::string_view             key,
                const Item                  &item,
                std::span<const float>       query,
                SearchOptions                options) {
           typename Backend::config_type;
   
           { Backend::create(cfg) } -> std::same_as<common::Result<void>>;
           { Backend::destroy(cfg) } -> std::same_as<common::Result<void>>;
   
           { backend.open(cfg) } -> std::same_as<common::Result<void>>;
           { backend.close() } -> std::same_as<common::Result<void>>;
   
           { backend.contains(key) } -> std::same_as<common::Result<bool>>;
           { backend.get_item(key) } -> std::same_as<common::Result<Item>>;
           { backend.upsert_item(item) } -> std::same_as<common::Result<void>>;
           { backend.erase_item(key) } -> std::same_as<common::Result<void>>;
           { backend.search(query, options) }
           -> std::same_as<common::Result<std::vector<SearchHit>>>;
           { backend.list_keys() } -> std::same_as<common::Result<std::vector<Key>>>;
       };
   
   } // namespace PDJE_UTIL::db::nearest
