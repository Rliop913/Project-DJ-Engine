#pragma once

#include <vector>
#include <string>

using STR = std::string;

struct Position{
    unsigned long frame_in;
    unsigned long frame_out;
};

template<typename F, typename S, typename THR>
struct Values{
    F first;
    S second;
    THR third;
    Position pos;
};

class Meta{
public:
    void declare(std::string, std::string){
        ;//nothing. this is for faust
    }
};

class UI{
public:
    void openVerticalBox(std::string){
        ;//nothing. this is for faust
    }
    void closeBox(){
        ;//nothing. this is for faust
    }
};