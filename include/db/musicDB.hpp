#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>

#include "dbState.hpp"


struct musdata{
public:
    std::string title;
    std::string composer;
    std::string musicPath;
    BIN bpmBinary;
    double bpm = -1.0;
    std::string firstBar;

    musdata(stmt* dbstate);
    musdata(
        const std::string& title__ = "",
        const std::string& composer__ = "",
        const std::string& musicPath__ = "",
        const double bpm__ = -1.0
    );
    bool GenSearchSTMT(stmt& dbstate, sqlite3* db);
    bool GenInsertSTMT(stmt& dbstate, sqlite3* db);
};


