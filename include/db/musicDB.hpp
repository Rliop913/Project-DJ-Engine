#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>



struct musdata{
public:
    std::string title;
    std::string composer;
    std::string musicPath;
    std::string bpmBinary;
    double bpm = -1.0;

    musdata(sqlite3_stmt* stmt);
    musdata(
        const std::string& title__ = "",
        const std::string& composer__ = "",
        const std::string& musicPath__ = "",
        const double bpm__ = -1.0
    );
    bool GenSearchSTMT(sqlite3_stmt*& stmt, sqlite3* db) const;
    bool GenInsertSTMT(sqlite3_stmt*& stmt, sqlite3* db) const;
    // std::string GenInsertSQL() const;//need to implement
};


