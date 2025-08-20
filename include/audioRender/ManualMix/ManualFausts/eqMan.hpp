#pragma once
#include "DeckData.hpp"

class EQMan{
public:
    int EQLow = 0;
    int EQMid = 0;
    int EQHigh = 0;
    ARGSETTER
    makeArgSetter(){
        return {
            {"EQLow", [this](double value) {
                this->EQLow = static_cast<int>(value);
            }},
            {"EQMid", [this](double value) {
                this->EQMid = static_cast<int>(value);
            }},
            {"EQHigh", [this](double value) {
                this->EQHigh = static_cast<int>(value);
            }}
        };
    }
};