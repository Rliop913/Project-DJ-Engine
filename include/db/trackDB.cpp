#include "trackDB.hpp"

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
        return false;
    }
    if(trackTitle == ""){
        dbstate.bind_null(1);
    }
    dbstate.bind_text(2, trackTitle);
    
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
    dbstate.bind_text(1, trackTitle);
    dbstate.bind_blob(2, mixBinary);
    dbstate.bind_blob(3, noteBinary);
    dbstate.bind_text(4, cachedMixList);
    return true;
}