#include "musicDB.hpp"


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
        dbstate.bind_null(1);
    }
    if(composer == ""){
        dbstate.bind_null(3);
    }
    if(musicPath == ""){
        dbstate.bind_null(5);
    }
    if(bpm < 0){
        dbstate.bind_null(7);
    }
    dbstate.bind_text(2, title);
    dbstate.bind_text(4, composer);
    dbstate.bind_text(6, musicPath);
    dbstate.bind_double(8, bpm);
    
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
    dbstate.bind_text(1, title);
    dbstate.bind_text(2, composer);
    dbstate.bind_text(3, musicPath);
    dbstate.bind_double(4, bpm);
    dbstate.bind_blob(5, bpmBinary);
    dbstate.bind_text(6, firstBar);

    return true;

}