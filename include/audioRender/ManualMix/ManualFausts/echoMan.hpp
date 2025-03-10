#pragma once
#include "DeckData.hpp"
class EchoMan {
public:
    float EchoBps;
    float EchoFeedback;
    float EchoDryWet;
    ARGSETTER
    makeArgSetter(){
        return {
            {"EchoBps", [this](double value){
                this->EchoBps = static_cast<float>(value);
            }},
            {"EchoFeedback", [this](double value){
                this->EchoFeedback = static_cast<float>(value);
            }},
            {"EchoDryWet", [this](double value){
                this->EchoDryWet = static_cast<float>(value);
            }},
        };
    }
};