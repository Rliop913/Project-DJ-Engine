
.. _program_listing_file_include_util_db_nearest_Index.hpp:

Program Listing for File Index.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_nearest_Index.hpp>` (``include/util/db/nearest/Index.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/db/nearest/BackendConcept.hpp"
   
   #include <utility>
   
   namespace PDJE_UTIL::db::nearest {
   
   template <NearestNeighborBackendConcept Backend> class NearestNeighborIndex {
     public:
       using backend_type = Backend;
       using config_type  = typename Backend::config_type;
   
       static common::Result<void>
       create(const config_type &cfg)
       {
           return Backend::create(cfg);
       }
   
       static common::Result<void>
       destroy(const config_type &cfg)
       {
           return Backend::destroy(cfg);
       }
   
       static common::Result<NearestNeighborIndex>
       open(const config_type &cfg)
       {
           NearestNeighborIndex index;
           auto                 opened = index.backend_.open(cfg);
           if (!opened.ok()) {
               return common::Result<NearestNeighborIndex>::failure(opened.status());
           }
           index.is_open_ = true;
           return common::Result<NearestNeighborIndex>::success(std::move(index));
       }
   
       NearestNeighborIndex() = default;
       NearestNeighborIndex(NearestNeighborIndex &&other) noexcept
           : backend_(std::move(other.backend_)),
             is_open_(std::exchange(other.is_open_, false))
       {
       }
   
       NearestNeighborIndex &
       operator=(NearestNeighborIndex &&other) noexcept
       {
           if (this != &other) {
               if (is_open_) {
                   (void)close();
               }
               backend_ = std::move(other.backend_);
               is_open_ = std::exchange(other.is_open_, false);
           }
           return *this;
       }
   
       NearestNeighborIndex(const NearestNeighborIndex &) = delete;
       NearestNeighborIndex &
       operator=(const NearestNeighborIndex &) = delete;
   
       ~NearestNeighborIndex()
       {
           if (is_open_) {
               (void)close();
           }
       }
   
       common::Result<void>
       close()
       {
           if (!is_open_) {
               return common::Result<void>::success();
           }
           auto closed = backend_.close();
           if (closed.ok()) {
               is_open_ = false;
           }
           return closed;
       }
   
       bool
       is_open() const noexcept
       {
           return is_open_;
       }
   
       common::Result<bool>
       contains(std::string_view id) const
       {
           return backend_.contains(id);
       }
   
       common::Result<Item>
       get_item(std::string_view id) const
       {
           return backend_.get_item(id);
       }
   
       common::Result<void>
       upsert_item(const Item &item)
       {
           return backend_.upsert_item(item);
       }
   
       common::Result<void>
       erase_item(std::string_view id)
       {
           return backend_.erase_item(id);
       }
   
       common::Result<std::vector<SearchHit>>
       search(std::span<const float> query, SearchOptions options = {}) const
       {
           return backend_.search(query, options);
       }
   
       common::Result<std::vector<Key>>
       list_keys() const
       {
           return backend_.list_keys();
       }
   
       backend_type &
       backend() noexcept
       {
           return backend_;
       }
   
       const backend_type &
       backend() const noexcept
       {
           return backend_;
       }
   
     private:
       Backend backend_;
       bool    is_open_ = false;
   };
   
   } // namespace PDJE_UTIL::db::nearest
