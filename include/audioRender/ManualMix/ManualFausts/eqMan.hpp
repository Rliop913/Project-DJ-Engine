#pragma once
#include "DeckData.hpp"

class EQMan{
public:
    int EQSelect;
    int EQPower;
    ARGSETTER
    makeArgSetter(){
        return {
            {"EQSelect", [this](double value) {
                this->EQSelect = static_cast<int>(value);
            }},
            {"EQPower", [this](double value) {
                this->EQPower = static_cast<int>(value);
            }}
        };
    }
};