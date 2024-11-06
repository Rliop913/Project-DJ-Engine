#pragma once
#include "dbRoot.hpp"


struct musdata : dbOut{
private:
    std::string title;
    std::string composer;
    std::string musicPath;
    std::string bpmBinary;
    double bpm;
public:
    void FillData(sqlite3_stmt* stmt);
};

using MUSE = std::vector<musdata>;
using MAYBE_MUSE = std::optional<MUSE>;

class musedb : litedb{
    MAYBE_MUSE operator>>(const std::string& returnSQL);
};
