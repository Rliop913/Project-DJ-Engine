#include "dbRoot.hpp"
#include <iostream>

int
main()
{
    litedb dbr = litedb();
    if(!dbr.openDB("./tempdb.db")){
        return 1;
    }
    auto td = trackdata("first");
    auto md = musdata("WTC", "TEST", "./WTC.wav", 175);
    md.firstBar = "1056";
    BIN temp(10);
    temp[0]= 'f';
    temp[1]= 'f';
    temp[2]= 'f';
    temp[3]= 'f';
    temp[4]= 'f';
    temp[5]= 'f';
    temp[6]= 'f';
    temp[7]= 'f';
    temp[8]= 'f';
    temp[9]= 'f';
    
    
    md.bpmBinary = temp;
    dbr <= md;
    td.cachedMixList = "fda";
    td.mixBinary = temp;
    td.noteBinary = temp;
    dbr <= td;
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