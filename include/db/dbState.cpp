#include "dbState.hpp"
#include <cstring>
stmt::stmt()
{
}

stmt::~stmt()
{
    if(S != nullptr){
        sqlite3_finalize(S);
    }
}

int
stmt::bind_null(int idx)
{
    return sqlite3_bind_null(S, idx);
}

int 
stmt::bind_text(int idx, std::string& str)
{
    
    return sqlite3_bind_text(S, idx, str.c_str(), str.size(), SQLITE_TRANSIENT);
}
#include <iostream>
int 
stmt::bind_u8text(int idx, std::u8string& str)
{
    auto tempStr = std::string(str.begin(), str.end());
    std::cout << "DEBUGLINE dbState.cpp:31 "<< tempStr << std::endl;
    return sqlite3_bind_text(S, idx, tempStr.c_str(), tempStr.size(), SQLITE_TRANSIENT);
}
int 
stmt::bind_blob(int idx, BIN& bin)
{
    return sqlite3_bind_blob(S, idx, bin.data(), bin.size(), SQLITE_TRANSIENT);
}

int 
stmt::bind_double(int idx, double num)
{
    return sqlite3_bind_double(S, idx, num);
}

int 
stmt::bind_int(int idx, double num)
{
    return sqlite3_bind_int(S, idx, num);
}



bool
stmt::activate(sqlite3* db)
{
    return (sqlite3_prepare_v2(db, placeHold.c_str(), -1, &S, nullptr) == SQLITE_OK);
}

template<>
int
stmt::colGet<COL_TYPE::INT>(int idx)
{
    return sqlite3_column_int(S, idx);
}

template<>
double
stmt::colGet<COL_TYPE::DOUBLE>(int idx)
{
    return sqlite3_column_double(S, idx);
}


template<>
std::u8string
stmt::colGet<COL_TYPE::TEXT>(int idx)
{   
    auto ptr = sqlite3_column_text(S, idx);
    auto sz = sqlite3_column_bytes(S, idx);
    if(sz == 0) return std::u8string();
    std::u8string tempstr(ptr, ptr + sz);
    std::cout << "DEBUGLINE dbState.cpp:81 "<< std::string(ptr, ptr+sz) << std::endl;
    return tempstr;
}


template<>
BIN
stmt::colGet<COL_TYPE::BLOB>(int idx)
{
    auto ptr = sqlite3_column_blob(S, idx);
    auto sz = sqlite3_column_bytes(S, idx);
    
    if(sz != 0){
        return BIN(static_cast<const u_int8_t*>(ptr), static_cast<const u_int8_t*>(ptr) + sz);
    }
    return BIN();
}