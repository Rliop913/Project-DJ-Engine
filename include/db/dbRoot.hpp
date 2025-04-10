#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>

#include "musicDB.hpp"
#include "trackDB.hpp"

/// music data vector
using MUS_VEC = std::vector<musdata>;
/// music data vector. check before use.
using MAYBE_MUS_VEC = std::optional<MUS_VEC>;

/// track data vector
using TRACK_VEC = std::vector<trackdata>;
/// track data vector. check before use.
using MAYBE_TRACK_VEC = std::optional<TRACK_VEC>;

/**
 * @brief the Root database Object
 * 
 */
class litedb{
private:
    /// the path to the database
    std::string ROOT_PATH;
    /// sqlite pointer
    sqlite3* db = nullptr;
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
     * @brief opens the Root Database and initialize.
     * 
     * @param dbPath path to Root Database
     * @return true 
     * @return false 
     */
    bool openDB(const std::string& dbPath);
    
    /**
     * @brief Get path to Root database
     * 
     * @return const std::string 
     */
    const std::string 
    getRoot(){
        return ROOT_PATH;
    }

    litedb();
    ~litedb();
};

template<typename DBType>
std::optional<std::vector<DBType>>
litedb::operator<<(DBType& searchClue)
{
    stmt dbstate = stmt();
    if(searchClue.GenSearchSTMT(dbstate, db)){
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
    stmt dbstate = stmt();
    if(insertObject.GenInsertSTMT(dbstate, db)){
        auto insertRes = sqlite3_step(dbstate.S);
        if(insertRes != SQLITE_DONE){
            return false;
        }
        return true;
    }
    return false;
}