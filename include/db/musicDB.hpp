#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>



struct musdata{
private:
    std::string title;
    std::string composer;
    std::string musicPath;
    std::string bpmBinary;
    double bpm = -1.0;
public:
    void FillData(sqlite3_stmt* stmt);
    std::string MatchSearch();
    
};


