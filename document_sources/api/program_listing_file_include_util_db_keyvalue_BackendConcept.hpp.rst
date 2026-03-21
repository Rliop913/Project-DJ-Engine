
.. _program_listing_file_include_util_db_keyvalue_BackendConcept.hpp:

Program Listing for File BackendConcept.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_keyvalue_BackendConcept.hpp>` (``include/util/db/keyvalue/BackendConcept.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/Result.hpp"
   #include "util/db/DbTypes.hpp"
   
   #include <concepts>
   #include <span>
   #include <string_view>
   #include <vector>
   
   namespace PDJE_UTIL::db::keyvalue {
   
   template <class Backend>
   concept KeyValueBackendConcept = requires(typename Backend::config_type cfg,
                                             Backend                      backend,
                                             std::string_view             key,
                                             std::span<const std::byte>   bytes,
                                             std::string_view             text) {
       typename Backend::config_type;
   
       { Backend::create(cfg) } -> std::same_as<common::Result<void>>;
       { Backend::destroy(cfg) } -> std::same_as<common::Result<void>>;
   
       { backend.open(cfg) } -> std::same_as<common::Result<void>>;
       { backend.close() } -> std::same_as<common::Result<void>>;
   
       { backend.contains(key) } -> std::same_as<common::Result<bool>>;
       { backend.get_text(key) } -> std::same_as<common::Result<Text>>;
       { backend.get_bytes(key) } -> std::same_as<common::Result<Bytes>>;
       { backend.put_text(key, text) } -> std::same_as<common::Result<void>>;
       { backend.put_bytes(key, bytes) } -> std::same_as<common::Result<void>>;
       { backend.erase(key) } -> std::same_as<common::Result<void>>;
       { backend.list_keys(std::string_view {}) }
       -> std::same_as<common::Result<std::vector<Key>>>;
   };
   
   } // namespace PDJE_UTIL::db::keyvalue
