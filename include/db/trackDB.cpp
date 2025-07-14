#include "trackDB.hpp"
// #include "errorTable.hpp"
#include "fileNameSanitizer.hpp"
#include <source_location>
#include "PDJE_LOG_SETTER.hpp"
#define CHK_BIND(res)\
if(res != SQLITE_OK){\
    auto now = std::source_location::current();\
    critlog("failed on sqlite.");\
    critlog(now.file_name());\
    critlog(now.line());\
    critlog(now.function_name());\
    critlog(sqlite3_errmsg(db));\
    return false;\
}


trackdata::trackdata(stmt* dbstate)
{
    trackTitle = dbstate->colGet<COL_TYPE::TEXT, std::string>(0);
    
    mixBinary = dbstate->colGet<COL_TYPE::BLOB, BIN>(1);
    
    noteBinary = dbstate->colGet<COL_TYPE::BLOB, BIN>(2);
    
    cachedMixList = dbstate->colGet<COL_TYPE::TEXT, std::string>(3);
}

trackdata::trackdata(const std::string& trackTitle__)
{
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(trackTitle__);
    if(!safeTitle){
        critlog("failed to sanitize filename. from trackdata(tracktitle). ErrtrackTitle: ");
        critlog(trackTitle__);
        return;
    }
    trackTitle = safeTitle.value();
}

bool 
trackdata::GenSearchSTMT(stmt& dbstate, sqlite3* db)
{
    dbstate.placeHold
    =
    "SELECT * FROM TRACK "
    "WHERE (? = -1 OR TrackTitle = ?);"
    ;
    if(!dbstate.activate(db)){
        critlog("failed to execute sql. from trackdata GenSearchSTMT.");
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
        critlog("failed to execute sql. from trackdata GenInsertSTMT.");
        return false;
    }
    CHK_BIND( dbstate.bind_text(1, trackTitle));
    CHK_BIND( dbstate.bind_blob(2, mixBinary));
    CHK_BIND( dbstate.bind_blob(3, noteBinary));
    CHK_BIND( dbstate.bind_text(4, cachedMixList));
    return true;
}


bool
trackdata::GenEditSTMT(stmt& dbstate, sqlite3* db, trackdata& toEdit)
{
    dbstate.placeHold
    =
    "UPDATE TRACK "
    "SET TrackTitle = ?, MixBinary = ?, NoteBinary = ?, CachedMixList = ? "
    "WHERE TrackTitle = ?; ";

    if(!dbstate.activate(db)) {
        critlog("failed to execute sql. from trackdata GenEditSTMT.");
        return false;
    }
    
    CHK_BIND(dbstate.bind_text  (1, toEdit.trackTitle   ))
    CHK_BIND(dbstate.bind_blob  (2, toEdit.mixBinary    ))
    CHK_BIND(dbstate.bind_blob  (3, toEdit.noteBinary   ))
    CHK_BIND(dbstate.bind_text  (4, toEdit.cachedMixList))
    CHK_BIND(dbstate.bind_text  (5, trackTitle          ))
    
    return true;

}


bool 
trackdata::GenDeleteSTMT(stmt& dbstate, sqlite3* db)
{
    dbstate.placeHold
    =
    "DELETE FROM TRACK "
    "WHERE TrackTitle = ?; ";

    if(!dbstate.activate(db)){
        critlog("failed to execute sql. from trackdata GenDeleteSTMT.");
        return false;
    }

    CHK_BIND(dbstate.bind_text(1, trackTitle))
    
    return true;
}

#undef CHK_BIND