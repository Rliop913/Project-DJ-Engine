#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/table.h>
#include <rocksdb/filter_policy.h>


#include "musicDB.hpp"
#include "trackDB.hpp"
#include <filesystem>
#include "PDJE_EXPORT_SETTER.hpp"

namespace fs = std::filesystem;

/// music data vector
using MUS_VEC = std::vector<musdata>;
/// music data vector. check before use.
using MAYBE_MUS_VEC = std::optional<MUS_VEC>;

/// track data vector
using TRACK_VEC = std::vector<trackdata>;
/// track data vector. check before use.
using MAYBE_TRACK_VEC = std::optional<TRACK_VEC>;

namespace RDB = ROCKSDB_NAMESPACE;
/**
 * @brief the Root database Object
 * 
 */
class PDJE_API litedb{
private:
    /// the path to the database
    fs::path ROOT_PATH;
    fs::path sqldbPath;
    fs::path kvdbPath;
    fs::path vectordbPath;
    /// sqlite pointer
    sqlite3* sdb = nullptr;
    RDB::DB* kvdb = nullptr;
    RDB::WriteOptions wops;
    RDB::ReadOptions rops;

    /// @brief checkes tables.
    /// @return OK / Not OK
    bool CheckTables();
public:
    /**
     * @brief the search query
     * 
     * @tparam DBType music or track data
     * @param searchClue the filters
     * @return std::optional<std::vector<DBType>> the result. check before use.
     */
    template<typename DBType>
    std::optional<std::vector<DBType>>
    operator<<(DBType& searchClue);

    /**
     * @brief the insert query
     * 
     * @tparam DBType music or track data
     * @param insertObject the data
     * @return true 
     * @return false 
     */
    template<typename DBType>
    bool
    operator<=(DBType& insertObject);

    /**
     * @brief Deletes data from the database.
     *
     * @tparam DBType The type of database object (music or track data).
     * @param deleteObject The object to delete.
     * @return true if the data was deleted successfully, false otherwise.
     */
    template<typename DBType>
    bool
    DeleteData(DBType& deleteObject);

    /**
     * @brief Edits data in the database.
     *
     * @tparam DBType The type of database object (music or track data).
     * @param searchObject The object to search for.
     * @param editObject The object with updated data.
     * @return true if the data was edited successfully, false otherwise.
     */
    template<typename DBType>
    bool
    EditData(DBType& searchObject, DBType& editObject); //to-do impl

    /**
     * @brief Retrieves a value from the key-value store.
     *
     * @param K The key to retrieve.
     * @param V The output parameter for the retrieved value.
     * @return true if the key was found and value retrieved, false otherwise.
     */
    bool
    KVGet(const SANITIZED& K, DONT_SANITIZE& V);

    /**
     * @brief Puts a key-value pair into the key-value store.
     *
     * @param K The key to insert or update.
     * @param V The value to associate with the key.
     * @return true if the operation was successful, false otherwise.
     */
    bool
    KVPut(const SANITIZED& K, const DONT_SANITIZE& V);

    /**
     * @brief opens the Root Database and initialize.
     * 
     * @param dbPath path to Root Database
     * @return true 
     * @return false 
     */
    bool openDB(const fs::path& dbPath);
    
    /**
     * @brief Get path to Root database
     * 
     * @return const std::string 
     */
    const fs::path
    getRoot(){
        return ROOT_PATH;
    }

    /**
     * @brief Constructs a new litedb object.
     *
     */
    litedb();
    /**
     * @brief Destroys the litedb object.
     *
     */
    ~litedb();
};

template<typename DBType>
std::optional<std::vector<DBType>>
litedb::operator<<(DBType& searchClue)
{
    stmt dbstate = stmt();
    if(searchClue.GenSearchSTMT(dbstate, sdb)){
        std::vector<DBType> data;
        while(sqlite3_step(dbstate.S) == SQLITE_ROW){
            data.emplace_back(&dbstate);
        }
        return std::move(data);
    }
    else{
        return std::nullopt;
    }
}
template<typename DBType>
bool
litedb::operator<=(DBType& insertObject)
{
    sqlite3_exec(sdb, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    stmt dbstate = stmt();
    if(insertObject.GenInsertSTMT(dbstate, sdb)){
        auto insertRes = sqlite3_step(dbstate.S);
        if(insertRes != SQLITE_DONE){
            sqlite3_exec(sdb, "ROLLBACK;", NULL, NULL, NULL);
            return false;
        }
        sqlite3_exec(sdb, "COMMIT;", nullptr, nullptr, NULL);
        return true;
    }
    return false;
}

template<typename DBType>
bool
litedb::DeleteData(DBType& deleteObject)
{
    stmt dbstate = stmt();
    if(deleteObject.GenDeleteSTMT(dbstate, sdb)){
        auto deleteRes = sqlite3_step(dbstate.S);
        if(deleteRes != SQLITE_DONE){
            return false;
        }
        return true;
    }
    return false;
}

template<typename DBType>
bool
litedb::EditData(DBType& searchObject, DBType& editObject)
{
    stmt dbstate = stmt();
    if(searchObject.GenEditSTMT(dbstate, sdb, editObject)){
        auto editRes = sqlite3_step(dbstate.S);
        if(editRes != SQLITE_DONE){
            return false;
        }
        return true;
    }
    return false;
}