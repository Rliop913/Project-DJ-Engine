#pragma once
#include "DeckData.hpp"

class PhaserMan{
public:
    float bps;
    float PhaserDryWet;
    ARGSETTER
    makeArgSetter(){
        return {
            {"bps", [this](double value) {
                this->bps = static_cast<float>(value);
            }},
            {"PhaserDryWet", [this](double value) {
                this->PhaserDryWet = static_cast<float>(value);
            }}
        };
    }
};