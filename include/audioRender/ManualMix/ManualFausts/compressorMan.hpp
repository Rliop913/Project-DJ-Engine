#pragma once
#include "DeckData.hpp"

class CompressorMan{
public:
    float strength;
    int threshDB;
    int attackMS;
    int releaseMS;
    int kneeDB;
    ARGSETTER
    makeArgSetter(){
        return{
            {"strength", [this](double value){
                this->strength = static_cast<float>(value);
            }},
            {"threshDB", [this](double value){
                this->threshDB = static_cast<int>(value);
            }},
            {"attackMS", [this](double value){
                this->attackMS = static_cast<int>(value);
            }},
            {"releaseMS", [this](double value){
                this->releaseMS = static_cast<int>(value);
            }},
            {"kneeDB", [this](double value){
                this->kneeDB = static_cast<int>(value);
            }}
        };
    }
};