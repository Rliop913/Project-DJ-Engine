#include <stdexcept>
#include "dbRoot.hpp"
#include "errorTable.hpp"


litedb::litedb(const std::string& dbPath)
{
    int res = sqlite3_open(dbPath.c_str(), &db);
    if (res != SQLITE_OK)
    {
        errpdje::ereport(
            "sqlite DB open ERR. ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::PTR_NO_EXISTS,
            "dbRoot litedb()"
            );
        throw std::runtime_error("failed to open sqlite, ERR ID : " + std::to_string(res));
    }
    CheckTables();
}

litedb::~litedb()
{
    sqlite3_close(db);
}

void
litedb::CheckTables()
{
    sqlite3_stmt* chk_mus;
    sqlite3_stmt* chk_trk;
    std::string msql = "PRAGMA table_info('MUSIC')";
    std::string tsql = "PRAGMA table_info('TRACK')";
    if (sqlite3_prepare_v2(db, msql.c_str(), -1, &chk_mus, nullptr) != SQLITE_OK) {
        errpdje::ereport(
            "sql prepare error SQL ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "dbRoot CheckTables music prepare"
            );
        throw std::runtime_error("failed to load chk mus sql table");
    }
    if (sqlite3_prepare_v2(db, tsql.c_str(), -1, &chk_trk, nullptr) != SQLITE_OK) {
        errpdje::ereport(
            "sql prepare error SQL ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "dbRoot CheckTables track prepare"
            );
        throw std::runtime_error("failed to load chk track sql table");
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
            errpdje::ereport(
            "Failed to execute sql. ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "dbRoot CheckTables music execute"
            );
            throw std::runtime_error("failed to make MUSIC table");
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
            errpdje::ereport(
            "Failed to execute sql. ERRNO: " + std::to_string(SQLITE_LAST_ERRNO), 
            errpdje::ERR_TYPE::SQL_ERROR,
            "dbRoot CheckTables track execute"
            );
            throw std::runtime_error("failed to make TRACK table");
        }
    }
    sqlite3_finalize(chk_mus);
    sqlite3_finalize(chk_trk);
}