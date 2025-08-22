#pragma once
#include "DeckData.hpp"

class FlangerMan{
public:
    float bps;
    float FlangerDryWet;
    ARGSETTER
    makeArgSetter(){
        return {
            {"Bps", [this](double value) {
                this->bps = static_cast<float>(value);
            }},
            {"FlangerDryWet", [this](double value) {
                this->FlangerDryWet = static_cast<float>(value);
            }}
        };
    }
};