#include "trackDB.hpp"

trackdata::trackdata(sqlite3_stmt* stmt)
{
    trackTitle =
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 0)
        )
    );
    mixBinary =
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_blob(stmt, 1)
        )
    );
    noteBinary =
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_blob(stmt, 2)
        )
    );
    cachedMixList =
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 3)
        )
    );
    
}

trackdata::trackdata(const std::string& trackTitle__)
:trackTitle(trackTitle__)
{
    
}

bool 
trackdata::GenSearchSTMT(sqlite3_stmt*& stmt, sqlite3* db)
const
{
    std::string placeHold
    =
    "SELECT TrackTitle FROM TRACK "
    "WHERE (? IS NULL OR TrackTitle = ?)"
    ;
    if(sqlite3_prepare_v2(db, placeHold.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        return false;
    }
    if(trackTitle == ""){
        sqlite3_bind_null(stmt, 1);
    }
    sqlite3_bind_text(stmt, 2, trackTitle.c_str(), -1, SQLITE_STATIC);
    
    return true;
}
