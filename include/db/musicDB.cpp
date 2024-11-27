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
}

std::string
musdata::MatchSearch()
{
    std::vector<std::string> wherejob;
    if(title != ""){
        wherejob.emplace_back("Title");
    } else if(composer != ""){
        wherejob.emplace_back("Composer");
    } else if(musicPath != ""){
        wherejob.emplace_back("MusicPath");
    } else if(bpm < 0){
        wherejob.emplace_back("Bpm");
    }
    std::string placeHold =
    "SELECT Title, Composer, MusicPath, Bpm, BpmBinary FROM MUSIC";
    
    if(!wherejob.empty()){
        placeHold += " WHERE ";
        for(int i=0; i < wherejob.size(); ++i){
            placeHold += wherejob[i];
            placeHold += " = ? ";
            if(i < (wherejob.size() - 1)){
                placeHold += "AND ";
            }
        }
    }
    placeHold += ";";
    return placeHold;
}