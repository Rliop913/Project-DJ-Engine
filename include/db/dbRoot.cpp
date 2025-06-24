#include <stdexcept>
#include "dbRoot.hpp"


litedb::litedb(){}

bool
litedb::openDB(const fs::path& dbPath)
{
    std::u8string u8str = dbPath.generic_u8string();
    std::string safeStr = std::string(u8str.begin(), u8str.end());
    if (sqlite3_open(safeStr.c_str(), &db) != SQLITE_OK){
        return false;
    }
    if(!CheckTables()){
        return false;
    }
    ROOT_PATH = dbPath;
    return true;
}

litedb::~litedb()
{
    if(db != nullptr){
        sqlite3_close(db);
    }
}

bool
litedb::CheckTables()
{
    sqlite3_stmt* chk_mus;
    sqlite3_stmt* chk_trk;
    std::string msql = "PRAGMA table_info('MUSIC')";
    std::string tsql = "PRAGMA table_info('TRACK')";
    if (sqlite3_prepare_v2(db, msql.c_str(), -1, &chk_mus, nullptr) != SQLITE_OK) {
        return false;
    }
    if (sqlite3_prepare_v2(db, tsql.c_str(), -1, &chk_trk, nullptr) != SQLITE_OK) {
        return false;
    }
    if(sqlite3_step(chk_mus) != SQLITE_ROW){
        std::string musmake =
        "CREATE TABLE MUSIC ( "
        "Title TEXT NOT NULL, "
        "Composer TEXT NOT NULL, "
        "MusicPath TEXT NOT NULL, "
        "Bpm DOUBLE NOT NULL, "
        "BpmBinary BLOB NOT NULL, "
        "FirstBar TEXT NOT NULL "
        ");";
        if(sqlite3_exec(db, musmake.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK){
            return false;
        }
    }
    if(sqlite3_step(chk_trk) != SQLITE_ROW){
        std::string trackmake =
        "CREATE TABLE TRACK ( "
        "TrackTitle TEXT NOT NULL, "
        "MixBinary BLOB NOT NULL, "
        "NoteBinary BLOB NOT NULL, "
        "CachedMixList TEXT NOT NULL "
        ");";
        if(sqlite3_exec(db, trackmake.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK){
            return false;
        }
    }
    sqlite3_finalize(chk_mus);
    sqlite3_finalize(chk_trk);
    return true;
}