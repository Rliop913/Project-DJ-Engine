#include "musicDB.hpp"
#include "errorTable.hpp"


#define CHK_BIND(res, error_type)\
if(res != SQLITE_OK){\
errpdje::ereport("sql bind errno: " + std::to_string(SQLITE_LAST_ERRNO), errpdje::ERR_TYPE::SQL_ERROR, ("musicDB bind " + std::string(error_type)));}



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
        errpdje::ereport(
            "sql prepare error SQL ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "musicDB gensearchSTMT"
            );
        return false;
    }
    if(title == ""){
        CHK_BIND(
        dbstate.bind_null(1),
        "NULL"
        )
    }
    if(composer == ""){
        CHK_BIND(
        dbstate.bind_null(3),
        "NULL"
        )
    }
    if(musicPath == ""){
        CHK_BIND(
        dbstate.bind_null(5),
        "NULL"
        )
    }
    if(bpm < 0){
        CHK_BIND(
        dbstate.bind_null(7),
        "NULL"
        )
    }
    CHK_BIND( dbstate.bind_text(2, title), "TEXT")
    CHK_BIND( dbstate.bind_text(4, composer), "TEXT")
    CHK_BIND( dbstate.bind_text(6, musicPath), "TEXT")
    CHK_BIND( dbstate.bind_double(8, bpm), "DOUBLE")
    
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
        errpdje::ereport(
            "sql prepare error SQL ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "musicDB genInsertSTMT"
            );
        return false;
    }
    CHK_BIND( dbstate.bind_text(1, title), "TEXT")
    CHK_BIND( dbstate.bind_text(2, composer), "TEXT")
    CHK_BIND( dbstate.bind_text(3, musicPath), "TEXT")
    CHK_BIND( dbstate.bind_double(4, bpm), "DOUBLE")
    CHK_BIND( dbstate.bind_blob(5, bpmBinary), "BLOB")
    CHK_BIND( dbstate.bind_text(6, firstBar), "TEXT")

    return true;

}
#undef CHK_BIND