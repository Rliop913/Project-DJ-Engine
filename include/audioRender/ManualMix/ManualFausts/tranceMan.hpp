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
            {"bps", [this](double value) {
                this->bps = static_cast<float>(value);
            }},
            {"gain", [this](double value) {
                this->gain = static_cast<float>(value);
            }},
            {"TranceDryWet", [this](double value) {
                this->TranceDryWet = static_cast<float>(value);
            }}
        };
    }
};