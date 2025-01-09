#pragma once
#include <sqlite3.h>
#include <string>

#include "dbState.hpp"
struct trackdata{
public:
    std::string trackTitle;
    BIN mixBinary;
    BIN noteBinary;
    std::string cachedMixList;

    trackdata(stmt* dbstate);
    trackdata(
        const std::string& trackTitle__ = ""
    );
    bool GenSearchSTMT(stmt& dbstate, sqlite3* db);
    bool GenInsertSTMT(stmt& dbstate, sqlite3* db);//need to implement
};