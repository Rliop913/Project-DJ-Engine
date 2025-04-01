#pragma once
#include "DeckData.hpp"

class RobotMan{
public:
    int robotFreq;
    float RobotDryWet;
    ARGSETTER
    makeArgSetter(){
        return {
            {"robotFreq", [this](double value) {
                 this->robotFreq = static_cast<int>(value);
            }},
            {"RobotDryWet", [this](double value) {
                 this->RobotDryWet = static_cast<float>(value);
            }}
        };
    }
};