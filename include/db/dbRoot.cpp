#include <stdexcept>
#include "dbRoot.hpp"


litedb::litedb(){}

bool
litedb::openDB(const fs::path& dbPath)
{
    // std::u8string u8str = dbPath.generic_u8string();
    if(!fs::is_directory(dbPath)){
        fs::create_directories(dbPath);
    }
    sqldbPath = dbPath / fs::path("sqlite.db");
    kvdbPath = dbPath / fs::path("rocksdb.db");
    vectordbPath = dbPath / fs::path("annoy.db");

    RDB::Options rdbops;
    rdbops.create_if_missing = true;
    rdbops.OptimizeForPointLookup(512 * 1024 * 1024);
    rdbops.OptimizeLevelStyleCompaction();
    RDB::BlockBasedTableOptions table_options;
    table_options.filter_policy.reset(RDB::NewBloomFilterPolicy(10, true));
    rdbops.table_factory.reset(NewBlockBasedTableFactory(table_options));
    rdbops.compression = RDB::kNoCompression;

    wops.sync = true;
    wops.disableWAL = false;

    rops.verify_checksums = true;
    rops.fill_cache = false;

    auto sqlRes = sqlite3_open(sqldbPath.generic_string().c_str(), &sdb);
    auto kvdbRes = RDB::DB::Open(rdbops, kvdbPath.generic_string(), &kvdb);

    if (sqlRes != SQLITE_OK){
        DB_ERROR+= "\nsqlite openErr: " + std::string(sqlite3_errmsg(sdb));
        return false;
    }
    if (!kvdbRes.ok()){
        DB_ERROR += "\nkvdb openErr: " + kvdbRes.ToString();
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
    if(sdb != nullptr){
        sqlite3_close(sdb);
    }
    if(kvdb != nullptr){
        delete kvdb;
    }
}

bool
litedb::CheckTables()
{
    sqlite3_stmt* chk_mus;
    sqlite3_stmt* chk_trk;
    std::string msql = "PRAGMA table_info('MUSIC')";
    std::string tsql = "PRAGMA table_info('TRACK')";
    if (sqlite3_prepare_v2(sdb, msql.c_str(), -1, &chk_mus, nullptr) != SQLITE_OK) {
        DB_ERROR+= "\nsql msql prepareErr: " + std::string(sqlite3_errmsg(sdb));
        return false;
    }
    if (sqlite3_prepare_v2(sdb, tsql.c_str(), -1, &chk_trk, nullptr) != SQLITE_OK) {
        DB_ERROR+= "\nsql tsql prepareErr: " + std::string(sqlite3_errmsg(sdb));
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
        if(sqlite3_exec(sdb, musmake.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK){
            DB_ERROR+= "\nsql create music execErr: " + std::string(sqlite3_errmsg(sdb));
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
        if(sqlite3_exec(sdb, trackmake.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK){
            DB_ERROR+= "\nsql create track execErr: " + std::string(sqlite3_errmsg(sdb));
            return false;
        }
    }
    sqlite3_finalize(chk_mus);
    sqlite3_finalize(chk_trk);
    return true;
}






bool
litedb::KVGet(const std::string& K, std::string& V)
{
    
    auto getRes = kvdb->Get(rops, K, &V);
    if(getRes.IsNotFound()){
        return false;
    }
    if(!getRes.ok()){
        DB_ERROR += "\nkvdb getErr: " + getRes.ToString();
        return false;
    }
    return true;
}

bool
litedb::KVPut(const std::string& K, const std::string& V)
{
    auto putRes = kvdb->Put(wops, K, V);
    if(!putRes.ok()){
        DB_ERROR += "\nkvdb putErr: " + putRes.ToString();
        return false;
    }
    return true;
}
