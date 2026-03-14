#include "dbRoot.hpp"

#include <cassert>

#include <memory>
#include <rocksdb/db.h>
#include <rocksdb/filter_policy.h>
#include <rocksdb/options.h>
#include <rocksdb/table.h>

#include <stdexcept>

#include "PDJE_LOG_SETTER.hpp"
namespace ROCK_DB = ROCKSDB_NAMESPACE;
class RDB {
  public:
    ROCK_DB::DB          *kvdb = nullptr;
    ROCK_DB::WriteOptions wops;
    ROCK_DB::ReadOptions  rops;
};

litedb::litedb() : kvdb_impl(std::make_unique<RDB>())
{
}

bool
litedb::openDB(const fs::path &dbPath)
{
    // std::u8string u8str = dbPath.generic_u8string();
    if (!fs::is_directory(dbPath)) {

        infolog(
            "directory not found. making new one. from litedb openDB. path: ",
            dbPath.generic_string());
        // infolog(dbPath.generic_string());
        fs::create_directories(dbPath);
    }
    sqldbPath    = dbPath / fs::path("sqlite.db");
    kvdbPath     = dbPath / fs::path("rocksdb.db");
    vectordbPath = dbPath / fs::path("annoy.db");

    ROCK_DB::Options rdbops;
    rdbops.create_if_missing = true;
    rdbops.OptimizeForPointLookup(512 * 1024 * 1024);
    rdbops.OptimizeLevelStyleCompaction();
    rdbops.file_checksum_gen_factory.reset();
    ROCK_DB::BlockBasedTableOptions table_options;
    table_options.filter_policy.reset(ROCK_DB::NewBloomFilterPolicy(10, true));
    rdbops.table_factory.reset(NewBlockBasedTableFactory(table_options));
    rdbops.compression = ROCK_DB::kNoCompression;

    kvdb_impl->wops.sync       = true;
    kvdb_impl->wops.disableWAL = false;

    kvdb_impl->rops.verify_checksums = true;
    kvdb_impl->rops.fill_cache       = false;

    auto sqlRes = sqlite3_open(sqldbPath.generic_string().c_str(), &sdb);

    sqlite3_exec(sdb, "PRAGMA synchronous   = FULL;", NULL, NULL, NULL);

    auto kvdbRes =
        ROCK_DB::DB::Open(rdbops, kvdbPath.generic_string(), &kvdb_impl->kvdb);

    if (sqlRes != SQLITE_OK) {
        critlog("failed to open sqlite. from litedb openDB. sqliteErrmsg: ");
        critlog(sqlite3_errmsg(sdb));
        assert(false);
        return false;
    }
    if (!kvdbRes.ok()) {
        critlog("failed to open rocksDB. from litedb openDB. rocksdbErrmsg: ");
        critlog(kvdbRes.ToString());
        assert(false);
        return false;
    }
    if (!CheckTables()) {
        critlog("failed to check tables. from litedb openDB.");
        return false;
    }
    ROOT_PATH = dbPath;
    return true;
}

litedb::~litedb()
{
    if (sdb != nullptr) {
        sqlite3_close(sdb);
    }
    if (kvdb_impl->kvdb != nullptr) {
        delete kvdb_impl->kvdb;
    }
}

bool
litedb::CheckTables()
{
    sqlite3_stmt *chk_mus;
    sqlite3_stmt *chk_trk;
    std::string   msql = "PRAGMA table_info('MUSIC')";
    std::string   tsql = "PRAGMA table_info('TRACK')";
    if (sqlite3_prepare_v2(sdb, msql.c_str(), -1, &chk_mus, nullptr) !=
        SQLITE_OK) {
        critlog("failed to prepare music sql. from litedb CheckTables. "
                "sqliteErrmsg: ");
        std::string sqlLog = sqlite3_errmsg(sdb);
        critlog(sqlLog);
        return false;
    }
    if (sqlite3_prepare_v2(sdb, tsql.c_str(), -1, &chk_trk, nullptr) !=
        SQLITE_OK) {
        critlog("failed to prepare track sql. from litedb CheckTables. "
                "sqliteErrmsg: ");
        std::string sqlLog = sqlite3_errmsg(sdb);
        critlog(sqlLog);
        return false;
    }
    if (sqlite3_step(chk_mus) != SQLITE_ROW) {
        std::string musmake = "CREATE TABLE MUSIC ( "
                              "Title TEXT NOT NULL, "
                              "Composer TEXT NOT NULL, "
                              "MusicPath TEXT NOT NULL, "
                              "Bpm DOUBLE NOT NULL, "
                              "BpmBinary BLOB NOT NULL, "
                              "FirstBeat TEXT NOT NULL "
                              ");";
        if (sqlite3_exec(sdb, musmake.c_str(), nullptr, nullptr, nullptr) !=
            SQLITE_OK) {
            critlog("failed to create music sqlite exec. from litedb "
                    "CheckTables. sqliteErrmsg: ");
            std::string sqlLog = sqlite3_errmsg(sdb);
            critlog(sqlLog);
            return false;
        }
    }
    if (sqlite3_step(chk_trk) != SQLITE_ROW) {
        std::string trackmake = "CREATE TABLE TRACK ( "
                                "TrackTitle TEXT NOT NULL, "
                                "MixBinary BLOB NOT NULL, "
                                "NoteBinary BLOB NOT NULL, "
                                "CachedMixList TEXT NOT NULL "
                                ");";
        if (sqlite3_exec(sdb, trackmake.c_str(), nullptr, nullptr, nullptr) !=
            SQLITE_OK) {
            critlog("failed to create track sqlite exec. from litedb "
                    "CheckTables. sqliteErrmsg: ");
            std::string sqlLog = sqlite3_errmsg(sdb);
            critlog(sqlLog);
            return false;
        }
    }
    sqlite3_finalize(chk_mus);
    sqlite3_finalize(chk_trk);
    return true;
}

bool
litedb::KVGet(const SANITIZED &K, DONT_SANITIZE &V)
{

    auto getRes = kvdb_impl->kvdb->Get(kvdb_impl->rops, K, &V);
    if (getRes.IsNotFound()) {
        warnlog(
            "cannot find music from database. from litedb KVGet. Keydata: ");
        warnlog(K);
        return false;
    }
    if (!getRes.ok()) {
        critlog("failed to get music from database. from litedb KVGet. "
                "rocksdbErr: ");
        std::string resErr = getRes.ToString();
        critlog(resErr);
        return false;
    }
    return true;
}

bool
litedb::KVPut(const SANITIZED &K, const DONT_SANITIZE &V)
{

    auto putRes = kvdb_impl->kvdb->Put(kvdb_impl->wops, K, V);
    if (!putRes.ok()) {
        critlog("failed to put new datas to database. from litedb KVPut. "
                "rocksdbErr: ");
        std::string resErr = putRes.ToString();
        critlog(resErr);
        return false;
    }
    return true;
}
