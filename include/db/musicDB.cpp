#include "musicDB.hpp"


musdata::musdata(sqlite3_stmt* stmt)
{
    title = std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 0)
        )
    );

    composer = std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 1)
        )
    );

    musicPath = std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 2)
        )
    );
    bpm = sqlite3_column_double(stmt, 3);

    bpmBinary = std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_blob(stmt, 4)
        )
    );
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
musdata::GenSearchSTMT(sqlite3_stmt*& stmt, sqlite3* db) 
const
{
    std::string placeHold 
    =
    "SELECT Title, Composer, MusicPath, Bpm, BpmBinary FROM MUSIC"
    " WHERE (? IS NULL OR Title = ?)"
    " AND (? IS NULL OR Composer = ?)"
    " AND (? IS NULL OR MusicPath = ?)"
    " AND (? IS NULL OR Bpm = ?)"
    ;
    if(sqlite3_prepare_v2(db, placeHold.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        return false;
    }
    if(title == ""){
        sqlite3_bind_null(stmt, 1);
    }
    if(composer == ""){
        sqlite3_bind_null(stmt, 3);
    }
    if(musicPath == ""){
        sqlite3_bind_null(stmt, 5);
    }
    if(bpm < 0){
        sqlite3_bind_null(stmt, 7);
    }
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, composer.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, musicPath.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 8, bpm);
    
    return true;
}

bool
musdata::GenInsertSTMT(sqlite3_stmt*& stmt, sqlite3* db)
const
{

}