
.. _program_listing_file_include_db_musicDB.cpp:

Program Listing for File musicDB.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_musicDB.cpp>` (``include/db/musicDB.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "musicDB.hpp"
   // #include "errorTable.hpp"
   
   
   #define CHK_BIND(res)\
   if(res != SQLITE_OK){\
   return false;\
   }
   // errpdje::ereport("sql bind errno: " + std::to_string(SQLITE_LAST_ERRNO), errpdje::ERR_TYPE::SQL_ERROR, ("musicDB bind " + std::string(error_type)));}
   
   
   
   musdata::musdata(stmt* dbstate)
   {
       title = dbstate->colGet<COL_TYPE::TEXT, std::string>(0);
       composer = dbstate->colGet<COL_TYPE::TEXT, std::string>(1);
       musicPath = dbstate->colGet<COL_TYPE::TEXT, std::string>(2);
       bpm = dbstate->colGet<COL_TYPE::DOUBLE, double>(3);
       bpmBinary = dbstate->colGet<COL_TYPE::BLOB, BIN>(4);
       firstBar = dbstate->colGet<COL_TYPE::TEXT, std::string>(5);
   }
   
   musdata::musdata(
       const std::string& title__,
       const std::string& composer__,
       const std::string& musicPath__,
       const double bpm__
   ):
   title(title__),
   composer(composer__),
   musicPath(musicPath__),
   bpm(bpm__)
   {}
   
   
   bool
   musdata::GenSearchSTMT(stmt& dbstate, sqlite3* db) 
   {
       dbstate.placeHold
       =
       "SELECT * FROM MUSIC"
       " WHERE (? = -1 OR Title = ?)"
       " AND (? = -1 OR Composer = ?)"
       " AND (? = -1 OR MusicPath = ?)"
       " AND (? = -1 OR Bpm = ?)"
       ;
       if(!dbstate.activate(db)){
           return false;
       }
       if(title == ""){
           CHK_BIND(
           dbstate.bind_int(1, -1)
           )
       }
       if(composer == ""){
           CHK_BIND(
           dbstate.bind_int(3, -1)
           )
       }
       if(musicPath == ""){
           CHK_BIND(
           dbstate.bind_int(5, -1)
           )
       }
       if(bpm < 0){
           CHK_BIND(
           dbstate.bind_int(7, -1)
           )
       }
       CHK_BIND( dbstate.bind_text(2, title))
       CHK_BIND( dbstate.bind_text(4, composer))
       CHK_BIND( dbstate.bind_text(6, musicPath))
       CHK_BIND( dbstate.bind_double(8, bpm))
       
       return true;
   }
   
   bool
   musdata::GenInsertSTMT(stmt& dbstate, sqlite3* db)
   {
       dbstate.placeHold
       =
       "INSERT INTO MUSIC "
       "( Title, Composer, MusicPath, Bpm, BpmBinary, FirstBar ) "
       "VALUES "
       "( ?, ?, ?, ?, ?, ?); ";
   
       if(!dbstate.activate(db)){
           return false;
       }
       CHK_BIND( dbstate.bind_text(1, title))
       CHK_BIND( dbstate.bind_text(2, composer))
       CHK_BIND( dbstate.bind_text(3, musicPath))
       CHK_BIND( dbstate.bind_double(4, bpm))
       CHK_BIND( dbstate.bind_blob(5, bpmBinary))
       CHK_BIND( dbstate.bind_text(6, firstBar))
   
       return true;
   
   }
   #undef CHK_BIND
