#pragma once
#include "DeckData.hpp"

class VolMan{
public:
    float VolPower;
    ARGSETTER
    makeArgSetter(){
        return {
            {"VolPower", [this](double value) {
                this->VolPower = static_cast<float>(value);
            }}
        };
    }
};