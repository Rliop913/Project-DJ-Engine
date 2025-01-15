#pragma once

#include <vector>

struct Position{
    unsigned long frame_in;
    unsigned long frame_out;
};
template<typename F, typename S, typename THR>
struct filter_Val{
    F first;
    S second;
    THR third;
    Position pos;
};


struct filter{
    std::vector<filter_Val<double, double, bool>> High;
    std::vector<filter_Val<double, double, bool>> Low;
};

struct DeckData{
    // std::vector<>
};

class Deck{

};