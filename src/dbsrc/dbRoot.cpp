#include <stdexcept>
#include "dbRoot.hpp"
void
dbOut::FillData(sqlite3_stmt* stmt)
{
    return;
}

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

std::optional<std::vector<dbOut>>
litedb::operator>>(const std::string& returnSQL)
{
    return std::nullopt;
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