#pragma once
#include <sqlite3.h>
#include <string>
using BIN = std::string;
struct trackdata{
public:
    std::string trackTitle;
    BIN mixBinary;
    BIN noteBinary;
    std::string cachedMixList;

    trackdata(sqlite3_stmt* stmt);
    trackdata(
        const std::string& trackTitle__ = ""
    );
    bool GenSearchSTMT(sqlite3_stmt*& stmt, sqlite3* db) const;
    std::string GenInsertSQL() const;//need to implement
};