
.. _program_listing_file_include_util_db_relational_Types.hpp:

Program Listing for File Types.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_relational_Types.hpp>` (``include/util/db/relational/Types.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/db/DbTypes.hpp"
   
   #include <cstdint>
   #include <optional>
   #include <stdexcept>
   #include <string>
   #include <string_view>
   #include <variant>
   #include <vector>
   
   namespace PDJE_UTIL::db::relational {
   
   enum class ValueKind {
       null_value,
       integer,
       real,
       text,
       bytes
   };
   
   using ValueStorage = std::variant<std::monostate, std::int64_t, double, Text, Bytes>;
   
   struct Value {
       ValueStorage storage {};
   
       ValueKind
       kind() const noexcept
       {
           switch (storage.index()) {
               case 0:
                   return ValueKind::null_value;
               case 1:
                   return ValueKind::integer;
               case 2:
                   return ValueKind::real;
               case 3:
                   return ValueKind::text;
               default:
                   return ValueKind::bytes;
           }
       }
   };
   
   using Params = std::vector<Value>;
   
   struct Row {
       std::vector<std::string> columns {};
       std::vector<Value>       values {};
   
       const Value *
       find(std::string_view column_name) const noexcept
       {
           for (std::size_t i = 0; i < columns.size() && i < values.size(); ++i) {
               if (columns[i] == column_name) {
                   return &values[i];
               }
           }
           return nullptr;
       }
   
       const Value &
       at(std::size_t index) const
       {
           return values.at(index);
       }
   };
   
   struct ExecResult {
       std::uint64_t               affected_rows     = 0;
       std::optional<std::int64_t> last_insert_rowid {};
   };
   
   struct QueryResult {
       std::vector<Row> rows {};
   };
   
   } // namespace PDJE_UTIL::db::relational
