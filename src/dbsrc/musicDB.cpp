#include "musicDB.hpp"

void
musdata::FillData(sqlite3_stmt* stmt)
{
    title = 
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 0)
        )
    );

    composer = 
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 1)
        )
    );

    musicPath = 
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 2)
        )
    );
    bpm =
    sqlite3_column_double(stmt, 3);

    bpmBinary =
    std::string(
        reinterpret_cast<const char*>(
            sqlite3_column_blob(stmt, 4)
        )
    );
    

    adsgasfsa
}


MAYBE_MUSE
musedb::operator>>(const std::string& returnSQL)
{
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, returnSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt);
        return std::nullopt;
    }
    else{
        MUSE data;
        while (sqlite3_step(stmt) == SQLITE_ROW){
            data.emplace_back();
            data.back().FillData(stmt);
        }
        sqlite3_finalize(stmt);
        return std::move(data);        
    }
}