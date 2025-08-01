
.. _program_listing_file_include_db_musicDB.cpp:

Program Listing for File musicDB.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_musicDB.cpp>` (``include/db/musicDB.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "musicDB.hpp"
   // #include "errorTable.hpp"
   #include <source_location>
   
   #include "PDJE_LOG_SETTER.hpp"
   
   
   
   #define CHK_BIND(res)\
   if(res != SQLITE_OK){\
       auto now = std::source_location::current();\
       critlog("failed on sqlite.");\
       critlog(now.file_name());\
       std::string lineNumber = std::to_string(now.line());\
       critlog(lineNumber);\
       critlog(now.function_name());\
       std::string sqlLog = sqlite3_errmsg(db);\
       critlog(sqlLog);\
       return false;\
   }
   
   musdata::musdata(stmt* dbstate)
   {
       title = dbstate->colGet<COL_TYPE::PDJE_TEXT, std::string>(0);
       composer = dbstate->colGet<COL_TYPE::PDJE_TEXT, std::string>(1);
       musicPath = dbstate->colGet<COL_TYPE::PDJE_TEXT, std::string>(2);
       bpm = dbstate->colGet<COL_TYPE::PDJE_DOUBLE, double>(3);
       bpmBinary = dbstate->colGet<COL_TYPE::PDJE_BLOB, BIN>(4);
       firstBar = dbstate->colGet<COL_TYPE::PDJE_TEXT, std::string>(5);
   }
   
   musdata::musdata(
       const UNSANITIZED& title__,
       const UNSANITIZED& composer__,
       const SANITIZED_ORNOT& musicPath__,
       const double bpm__
   ):
   musicPath(musicPath__),
   bpm(bpm__)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title__);
       auto safeComposer = PDJE_Name_Sanitizer::sanitizeFileName(composer__);
       if(!safeTitle || !safeComposer){
           critlog("failed to sanitize filename. from musdata(title, composer, muspath, bpm). TileComposer: ");
           critlog(title__);
           critlog(composer__);
           return;
       }
       title = safeTitle.value();
       composer = safeComposer.value();
   }
   
   
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
   
   bool
   musdata::GenEditSTMT(stmt& dbstate, sqlite3* db, musdata& toEdit)
   {
       dbstate.placeHold
       =
       "UPDATE MUSIC "
       "SET Title = ?, Composer = ?, MusicPath = ?, Bpm = ?, BpmBinary = ?, FirstBar = ? "
       "WHERE Title = ? AND Composer = ? AND MusicPath = ? AND Bpm = ?; ";
   
       if(!dbstate.activate(db)) return false;
       
       CHK_BIND(dbstate.bind_text   (1, toEdit.title    ))
       CHK_BIND(dbstate.bind_text   (2, toEdit.composer ))
       CHK_BIND(dbstate.bind_text   (3, toEdit.musicPath))
       CHK_BIND(dbstate.bind_double (4, toEdit.bpm      ))
       CHK_BIND(dbstate.bind_blob   (5, toEdit.bpmBinary))
       CHK_BIND(dbstate.bind_text   (6, toEdit.firstBar ))
       CHK_BIND(dbstate.bind_text   (7, title           ))
       CHK_BIND(dbstate.bind_text   (8, composer        ))
       CHK_BIND(dbstate.bind_text   (9, musicPath       ))
       CHK_BIND(dbstate.bind_double (10,bpm             ))
       
       return true;
   
   }
   
   
   bool 
   musdata::GenDeleteSTMT(stmt& dbstate, sqlite3* db)
   {
       dbstate.placeHold
       =
       "DELETE FROM MUSIC "
       "WHERE Title = ? AND Composer = ? AND MusicPath = ? AND Bpm = ?; ";
   
       if(!dbstate.activate(db)) return false;
   
       CHK_BIND(dbstate.bind_text   (1, title       ))
       CHK_BIND(dbstate.bind_text   (2, composer    ))
       CHK_BIND(dbstate.bind_text   (3, musicPath   ))
       CHK_BIND(dbstate.bind_double (4, bpm         ))
       
       return true;
   }
   
   
   #undef CHK_BIND
