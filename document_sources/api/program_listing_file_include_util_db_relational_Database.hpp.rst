
.. _program_listing_file_include_util_db_relational_Database.hpp:

Program Listing for File Database.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_relational_Database.hpp>` (``include/util/db/relational/Database.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/db/relational/BackendConcept.hpp"
   
   #include <utility>
   
   namespace PDJE_UTIL::db::relational {
   
   template <RelationalBackendConcept Backend> class RelationalDatabase {
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
   
       static common::Result<RelationalDatabase>
       open(const config_type &cfg)
       {
           RelationalDatabase db;
           auto               opened = db.backend_.open(cfg);
           if (!opened.ok()) {
               return common::Result<RelationalDatabase>::failure(opened.status());
           }
           db.is_open_ = true;
           return common::Result<RelationalDatabase>::success(std::move(db));
       }
   
       RelationalDatabase() = default;
       RelationalDatabase(RelationalDatabase &&other) noexcept
           : backend_(std::move(other.backend_)),
             is_open_(std::exchange(other.is_open_, false))
       {
       }
   
       RelationalDatabase &
       operator=(RelationalDatabase &&other) noexcept
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
   
       RelationalDatabase(const RelationalDatabase &) = delete;
       RelationalDatabase &
       operator=(const RelationalDatabase &) = delete;
   
       ~RelationalDatabase()
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
   
       common::Result<ExecResult>
       execute(std::string_view sql, const Params &params = {})
       {
           return backend_.execute(sql, params);
       }
   
       common::Result<QueryResult>
       query(std::string_view sql, const Params &params = {}) const
       {
           return backend_.query(sql, params);
       }
   
       common::Result<void>
       begin_transaction()
       {
           return backend_.begin_transaction();
       }
   
       common::Result<void>
       commit()
       {
           return backend_.commit();
       }
   
       common::Result<void>
       rollback()
       {
           return backend_.rollback();
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
   
   } // namespace PDJE_UTIL::db::relational
