#include "dbRoot.hpp"
#include <iostream>

int
main()
{
    litedb dbr("./tempdb.db");
    auto td = trackdata("first");
    auto md = musdata("test", "tt", "ttt", 123);
    auto mdret = dbr << md;
    auto tdret = dbr << td;
    if(!mdret.has_value()){
        
        return 1;
    }
    if(!tdret.has_value()){
        return 1;
    }
    for(auto i : mdret.value()){
        std::cout << i.title << ", " << i.musicPath 
        << ", " << i.composer << ", " << i.bpm << std::endl; 
    }
    for(auto i : tdret.value()){
        std::cout << i.trackTitle << std::endl; 
    }
    
    return 0;
}