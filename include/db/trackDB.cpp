#include "trackDB.hpp"
#include "errorTable.hpp"


#define CHK_BIND(res, error_type)\
if(res != SQLITE_OK){\
errpdje::ereport("sql bind errno: " + std::to_string(SQLITE_LAST_ERRNO), errpdje::ERR_TYPE::SQL_ERROR, ("trackDB bind " + std::string(error_type)));}


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
    "SELECT TrackTitle FROM TRACK "
    "WHERE (? IS NULL OR TrackTitle = ?)"
    ;
    if(!dbstate.activate(db)){
        errpdje::ereport(
            "sql prepare error SQL ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "trackDB gensearchSTMT"
            );
        return false;
    }
    if(trackTitle == ""){
        CHK_BIND((dbstate.bind_null(1)), "NULL")
    }
    CHK_BIND(
    dbstate.bind_text(2, trackTitle),
    "TEXT"
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
        errpdje::ereport(
            "sql prepare error SQL ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "trackDB genInsertSTMT"
            );
        return false;
    }
    CHK_BIND( dbstate.bind_text(1, trackTitle), "TEXT");
    CHK_BIND( dbstate.bind_blob(2, mixBinary), "BLOB");
    CHK_BIND( dbstate.bind_blob(3, noteBinary), "BLOB");
    CHK_BIND( dbstate.bind_text(4, cachedMixList), "TEXT");
    return true;
}
#undef CHK_BIND