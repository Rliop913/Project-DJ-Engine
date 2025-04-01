#pragma once
#include "DeckData.hpp"

class FilterMan{
public:
    int HLswitch;
    int Filterfreq;
    ARGSETTER
    makeArgSetter(){
        return {
            {"HLswitch", [this](double value) {
                this->HLswitch = static_cast<int>(value);
            }},
            {"Filterfreq", [this](double value) {
                this->Filterfreq = static_cast<int>(value);
            }}
        };
    }
};