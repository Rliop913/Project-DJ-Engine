#pragma once
#include "DeckData.hpp"

class TranceMan{
public:
    float bps;
    float gain;
    float TranceDryWet;
    ARGSETTER
    makeArgSetter(){
        return {
            {"Bps", [this](double value) {
                this->bps = static_cast<float>(value);
            }},
            {"Gain", [this](double value) {
                this->gain = static_cast<float>(value);
            }},
            {"TranceDryWet", [this](double value) {
                this->TranceDryWet = static_cast<float>(value);
            }}
        };
    }
};