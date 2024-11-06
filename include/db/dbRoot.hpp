#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>
struct dbOut{
    void FillData(sqlite3_stmt* stmt);
};
class litedb{
protected:
    sqlite3* db;
public:
    void operator<<(const std::string& insertSQL);
    std::optional<std::vector<dbOut>> operator>>(const std::string& returnSQL);
    litedb(const std::string& dbPath);
    ~litedb();
};
