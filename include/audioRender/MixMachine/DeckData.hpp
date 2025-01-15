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

struct Vol{
    std::vector<Values<double,double,bool>> Trim;
    std::vector<Values<double,double,bool>> Fader;
};