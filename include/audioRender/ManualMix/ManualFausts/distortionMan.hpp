#pragma once
#include "DeckData.hpp"

class DistortionMan{
public:
    float distortionValue;
    ARGSETTER
    makeArgSetter(){
        return{
            {"distortionValue", [this](double value){
                this->distortionValue = static_cast<float>(value);
            }}
        };
    }
};