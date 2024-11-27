#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>

#include "musicDB.hpp"

using MUSE = std::vector<musdata>;
using MAYBE_MUSE = std::optional<MUSE>;


class litedb{
protected:
    sqlite3* db;
public:
    void operator<<(const std::string& insertSQL);
    MAYBE_MUSE getMuseData(const std::string& returnSQL);
    


    litedb(const std::string& dbPath);
    ~litedb();
};
