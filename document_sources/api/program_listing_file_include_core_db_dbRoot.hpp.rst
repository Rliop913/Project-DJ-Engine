
.. _program_listing_file_include_core_db_dbRoot.hpp:

Program Listing for File dbRoot.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_dbRoot.hpp>` (``include\core\db\dbRoot.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <optional>
   #include <sqlite3.h>
   #include <string>
   #include <vector>
   
   #include <rocksdb/db.h>
   #include <rocksdb/filter_policy.h>
   #include <rocksdb/options.h>
   #include <rocksdb/table.h>
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "musicDB.hpp"
   #include "trackDB.hpp"
   #include <filesystem>
   
   namespace fs = std::filesystem;
   
   using MUS_VEC = std::vector<musdata>;
   using MAYBE_MUS_VEC = std::optional<MUS_VEC>;
   
   using TRACK_VEC = std::vector<trackdata>;
   using MAYBE_TRACK_VEC = std::optional<TRACK_VEC>;
   
   namespace RDB = ROCKSDB_NAMESPACE;
   class PDJE_API litedb {
     private:
       fs::path ROOT_PATH;
       fs::path sqldbPath;
       fs::path kvdbPath;
       fs::path vectordbPath;
       sqlite3          *sdb  = nullptr;
       RDB::DB          *kvdb = nullptr;
       RDB::WriteOptions wops;
       RDB::ReadOptions  rops;
   
       bool
       CheckTables();
   
     public:
       template <typename DBType>
       std::optional<std::vector<DBType>>
       operator<<(DBType &searchClue);
   
       template <typename DBType>
       bool
       operator<=(DBType &insertObject);
   
       template <typename DBType>
       bool
       DeleteData(DBType &deleteObject);
   
       template <typename DBType>
       bool
       EditData(DBType &searchObject, DBType &editObject); // to-do impl
   
       bool
       KVGet(const SANITIZED &K, DONT_SANITIZE &V);
   
       bool
       KVPut(const SANITIZED &K, const DONT_SANITIZE &V);
   
       bool
       openDB(const fs::path &dbPath);
   
       const fs::path
       getRoot()
       {
           return ROOT_PATH;
       }
   
       litedb();
       ~litedb();
   };
   
   template <typename DBType>
   std::optional<std::vector<DBType>>
   litedb::operator<<(DBType &searchClue)
   {
       stmt dbstate = stmt();
       if (searchClue.GenSearchSTMT(dbstate, sdb)) {
           std::vector<DBType> data;
           while (sqlite3_step(dbstate.S) == SQLITE_ROW) {
               data.emplace_back(&dbstate);
           }
           return std::move(data);
       } else {
           return std::nullopt;
       }
   }
   template <typename DBType>
   bool
   litedb::operator<=(DBType &insertObject)
   {
       sqlite3_exec(sdb, "BEGIN TRANSACTION;", NULL, NULL, NULL);
       stmt dbstate = stmt();
       if (insertObject.GenInsertSTMT(dbstate, sdb)) {
           auto insertRes = sqlite3_step(dbstate.S);
           if (insertRes != SQLITE_DONE) {
               sqlite3_exec(sdb, "ROLLBACK;", NULL, NULL, NULL);
               return false;
           }
           sqlite3_exec(sdb, "COMMIT;", nullptr, nullptr, NULL);
           return true;
       }
       return false;
   }
   
   template <typename DBType>
   bool
   litedb::DeleteData(DBType &deleteObject)
   {
       stmt dbstate = stmt();
       if (deleteObject.GenDeleteSTMT(dbstate, sdb)) {
           auto deleteRes = sqlite3_step(dbstate.S);
           if (deleteRes != SQLITE_DONE) {
               return false;
           }
           return true;
       }
       return false;
   }
   
   template <typename DBType>
   bool
   litedb::EditData(DBType &searchObject, DBType &editObject)
   {
       stmt dbstate = stmt();
       if (searchObject.GenEditSTMT(dbstate, sdb, editObject)) {
           auto editRes = sqlite3_step(dbstate.S);
           if (editRes != SQLITE_DONE) {
               return false;
           }
           return true;
       }
       return false;
   }
