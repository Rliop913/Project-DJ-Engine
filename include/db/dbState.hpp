#pragma once

#include <string>
#include <vector>

#include <sqlite3.h>


using BIN = std::vector<u_int8_t>;

enum COL_TYPE{
    INT,
    DOUBLE,
    TEXT,
    BLOB
};


struct stmt
{
public:

    std::string placeHold;

    sqlite3_stmt* S = nullptr;

    int bind_null(int idx);
    int bind_text(int idx, std::string& str);
    int bind_blob(int idx, BIN& bin);
    int bind_double(int idx, double num);
    int bind_int(int idx, double num);
    
    template<int T_COL, typename res>
    res
    colGet(int idx);

    bool activate(sqlite3* db);

    stmt();
    ~stmt();
};
