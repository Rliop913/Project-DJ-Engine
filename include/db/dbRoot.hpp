#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>

#include "musicDB.hpp"
#include "trackDB.hpp"

using MUS_VEC = std::vector<musdata>;
using MAYBE_MUS_VEC = std::optional<MUS_VEC>;

using TRACK_VEC = std::vector<trackdata>;
using MAYBE_TRACK_VEC = std::optional<TRACK_VEC>;

class litedb{
private:
    sqlite3* db;
    void CheckTables();
public:
    template<typename DBType>
    std::optional<std::vector<DBType>>
    operator<<(const DBType& searchClue);

    template<typename DBType>
    bool
    operator<<=(const DBType& insertObject);

    litedb(const std::string& dbPath);
    
    ~litedb();
};



template<typename DBType>
std::optional<std::vector<DBType>>
litedb::operator<<(const DBType& searchClue)
{
    sqlite3_stmt* stmt = nullptr;
    if(searchClue.GenSearchSTMT(stmt, db)){
        std::vector<DBType> data;
        while(sqlite3_step(stmt) == SQLITE_ROW){
            data.emplace_back(stmt);
        }
        sqlite3_finalize(stmt);
        return std::move(data);
    }
    else{
        sqlite3_finalize(stmt);
        return std::nullopt;
    }
}

template<typename DBType>
bool
litedb::operator<<=(const DBType& insertObject)
{

}