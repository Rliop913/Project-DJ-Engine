#pragma once

#include <vector>
#include <string>
#include <functional>
#include "PDJE_EXPORT_SETTER.hpp"

using STR = std::string;
using SETTER = std::function<void(double)>;
using ARGSETTER = std::unordered_map<std::string, SETTER>;
struct Position{
    unsigned long frame_in;
    unsigned long frame_out;
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