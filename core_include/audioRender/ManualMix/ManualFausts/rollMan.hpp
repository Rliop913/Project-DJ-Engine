#pragma once
#include "DeckData.hpp"

class RollMan {
  public:
    float RollBpm;
    float RollPower;
    ARGSETTER
    makeArgSetter()
    {
        return { { "RollBpm",
                   [this](double value) {
                       this->RollBpm = static_cast<float>(value);
                   } },
                 { "RollPower", [this](double value) {
                      this->RollPower = static_cast<float>(value);
                  } } };
    }
};