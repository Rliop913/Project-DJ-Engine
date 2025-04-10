
.. _program_listing_file_include_db_trackDB.cpp:

Program Listing for File trackDB.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_trackDB.cpp>` (``include/db/trackDB.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "trackDB.hpp"
   // #include "errorTable.hpp"
   
   
   #define CHK_BIND(res)\
   if(res != SQLITE_OK){\
   return false;\
   }
   // errpdje::ereport("sql bind errno: " + std::to_string(SQLITE_LAST_ERRNO), errpdje::ERR_TYPE::SQL_ERROR, ("trackDB bind " + std::string(error_type)));}
   
   
   trackdata::trackdata(stmt* dbstate)
   {
       trackTitle = dbstate->colGet<COL_TYPE::TEXT, std::string>(0);
       
       mixBinary = dbstate->colGet<COL_TYPE::BLOB, BIN>(1);
       
       noteBinary = dbstate->colGet<COL_TYPE::BLOB, BIN>(2);
       
       cachedMixList = dbstate->colGet<COL_TYPE::TEXT, std::string>(3);
       
   }
   
   trackdata::trackdata(const std::string& trackTitle__)
   :trackTitle(trackTitle__)
   {
       
   }
   
   bool 
   trackdata::GenSearchSTMT(stmt& dbstate, sqlite3* db)
   {
       dbstate.placeHold
       =
       "SELECT * FROM TRACK "
       "WHERE (? = -1 OR TrackTitle = ?)"
       ;
       if(!dbstate.activate(db)){
           return false;
       }
       if(trackTitle == ""){
           CHK_BIND((dbstate.bind_int(1, -1)))
       }
       CHK_BIND(
       dbstate.bind_text(2, trackTitle)
       )
       
       return true;
   }
   
   
   bool
   trackdata::GenInsertSTMT(stmt& dbstate, sqlite3* db)
   {
       dbstate.placeHold
       =
       "INSERT INTO TRACK "
       "(TrackTitle, MixBinary, NoteBinary, CachedMixList) "
       "VALUES "
       "(?, ?, ?, ?); ";
       if(!dbstate.activate(db)){
           return false;
       }
       CHK_BIND( dbstate.bind_text(1, trackTitle));
       CHK_BIND( dbstate.bind_blob(2, mixBinary));
       CHK_BIND( dbstate.bind_blob(3, noteBinary));
       CHK_BIND( dbstate.bind_text(4, cachedMixList));
       return true;
   }
   #undef CHK_BIND
