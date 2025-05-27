
.. _program_listing_file_include_db_dbRoot.hpp:

Program Listing for File dbRoot.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_dbRoot.hpp>` (``include/db/dbRoot.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <string>
   #include <vector>
   #include <optional>
   #include <sqlite3.h>
   
   #include "musicDB.hpp"
   #include "trackDB.hpp"
   
   using MUS_VEC = std::vector<musdata>;
   using MAYBE_MUS_VEC = std::optional<MUS_VEC>;
   
   using TRACK_VEC = std::vector<trackdata>;
   using MAYBE_TRACK_VEC = std::optional<TRACK_VEC>;
   
   class litedb{
   private:
       std::string ROOT_PATH;
       sqlite3* db = nullptr;
       bool CheckTables();
   public:
       template<typename DBType>
       std::optional<std::vector<DBType>>
       operator<<(DBType& searchClue);
   
       template<typename DBType>
       bool
       operator<=(DBType& insertObject);
   
       template<typename DBType>
       bool
       DeleteData(DBType& deleteObject);//to-do impl
   
       template<typename DBType>
       bool
       EditData(DBType& searchObject, DBType& editObject);//to-do impl
   
   
   
       bool openDB(const std::string& dbPath);
       
       const std::string 
       getRoot(){
           
           return ROOT_PATH;
       }
   
       litedb();
       ~litedb();
   };
   
   template<typename DBType>
   std::optional<std::vector<DBType>>
   litedb::operator<<(DBType& searchClue)
   {
       stmt dbstate = stmt();
       if(searchClue.GenSearchSTMT(dbstate, db)){
           std::vector<DBType> data;
           while(sqlite3_step(dbstate.S) == SQLITE_ROW){
               data.emplace_back(&dbstate);
           }
           return std::move(data);
       }
       else{
           return std::nullopt;
       }
   }
   
   template<typename DBType>
   bool
   litedb::operator<=(DBType& insertObject)
   {
       stmt dbstate = stmt();
       if(insertObject.GenInsertSTMT(dbstate, db)){
           auto insertRes = sqlite3_step(dbstate.S);
           if(insertRes != SQLITE_DONE){
               return false;
           }
           return true;
       }
       return false;
   }
   
   template<typename DBType>
   bool
   litedb::DeleteData(DBType& deleteObject)
   {
       stmt dbstate = stmt();
       if(deleteObject.GenDeleteSTMT(dbstate, db)){
           auto deleteRes = sqlite3_step(dbstate.S);
           if(deleteRes != SQLITE_DONE){
               return false;
           }
           return true;
       }
       return false;
   }
   
   template<typename DBType>
   bool
   litedb::EditData(DBType& searchObject, DBType& editObject)
   {
       stmt dbstate = stmt();
       if(searchObject.GenEditSTMT(dbstate, db, editObject)){
           auto editRes = sqlite3_step(dbstate.S);
           if(editRes != SQLITE_DONE){
               return false;
           }
           return true;
       }
       return false;
   }
