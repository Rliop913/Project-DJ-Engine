#include "dbState.hpp"
#include <cstring>
#include "PDJE_LOG_SETTER.hpp"
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
stmt::bind_text(int idx, SANITIZED_ORNOT& str)
{
    
    return sqlite3_bind_text(S, idx, str.c_str(), str.size(), SQLITE_TRANSIENT);
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
    bool activate_Res = (sqlite3_prepare_v2(db, placeHold.c_str(), -1, &S, nullptr) == SQLITE_OK);
    if(!activate_Res){
        critlog("failed to activate sql. from stmt activate. sqliteErr: ");
        std::string sqlLog = sqlite3_errmsg(db);
        critlog(sqlLog);
    }
    return activate_Res;
}

template<>
int
stmt::colGet<COL_TYPE::PDJE_INT>(int idx)
{
    return sqlite3_column_int(S, idx);
}

template<>
double
stmt::colGet<COL_TYPE::PDJE_DOUBLE>(int idx)
{
    return sqlite3_column_double(S, idx);
}


template<>
SANITIZED_ORNOT
stmt::colGet<COL_TYPE::PDJE_TEXT>(int idx)
{   
    auto ptr = sqlite3_column_text(S, idx);
    auto sz = sqlite3_column_bytes(S, idx);
    return SANITIZED_ORNOT(ptr, ptr + sz);
}


template<>
BIN
stmt::colGet<COL_TYPE::PDJE_BLOB>(int idx)
{
    auto ptr = sqlite3_column_blob(S, idx);
    auto sz = sqlite3_column_bytes(S, idx);
    
    if(sz != 0){
        return BIN(static_cast<const u_int8_t*>(ptr), static_cast<const u_int8_t*>(ptr) + sz);
    }
    warnlog("colget cannot return valid binary. from stmt colget-blob");
    return BIN();
}