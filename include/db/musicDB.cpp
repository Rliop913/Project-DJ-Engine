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
    "SELECT Title, Composer, MusicPath, Bpm, BpmBinary FROM MUSIC"
    " WHERE (? IS NULL OR Title = ?)"
    " AND (? IS NULL OR Composer = ?)"
    " AND (? IS NULL OR MusicPath = ?)"
    " AND (? IS NULL OR Bpm = ?)"
    ;
    if(!dbstate.activate(db)){
        return false;
    }
    if(title == ""){
        CHK_BIND(
        dbstate.bind_null(1)
        )
    }
    if(composer == ""){
        CHK_BIND(
        dbstate.bind_null(3)
        )
    }
    if(musicPath == ""){
        CHK_BIND(
        dbstate.bind_null(5)
        )
    }
    if(bpm < 0){
        CHK_BIND(
        dbstate.bind_null(7)
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