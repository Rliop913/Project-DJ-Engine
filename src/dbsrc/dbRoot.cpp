#include <stdexcept>
#include "dbRoot.hpp"


void
litedb::operator<<(const std::string& insertSQL)
{
    char* errMessage;
    if(sqlite3_exec(db, insertSQL.c_str(), nullptr, 0, &errMessage) != SQLITE_OK){
        std::string em(errMessage);
        sqlite3_free(errMessage);
        throw std::runtime_error(em.c_str());
    }
}

litedb::litedb(const std::string& dbPath)
{
    int res = sqlite3_open(dbPath.c_str(), &db);
    if (res != SQLITE_OK)
    {
        throw std::runtime_error("failed to open sqlite, ERR ID : " + std::to_string(res));
    }
}

litedb::~litedb()
{
    sqlite3_close(db);
}


MAYBE_MUSE
litedb::getMuseData(const std::string& returnSQL)
{
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, returnSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt);
        return std::nullopt;
    }
    else{
        MUSE data;
        while (sqlite3_step(stmt) == SQLITE_ROW){
            data.emplace_back();
            data.back().FillData(stmt);
        }
        sqlite3_finalize(stmt);
        return std::move(data);        
    }
}