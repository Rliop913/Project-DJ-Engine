#pragma once
#include "DeckData.hpp"

class PannerMan {
  public:
    float bps;
    float PGain;
    float PannerDryWet;
    ARGSETTER
    makeArgSetter()
    {
        return { { "Bps",
                   [this](double value) {
                       this->bps = static_cast<float>(value);
                   } },
                 { "PGain",
                   [this](double value) {
                       this->PGain = static_cast<float>(value);
                   } },
                 { "PannerDryWet", [this](double value) {
                      this->PannerDryWet = static_cast<float>(value);
                  } } };
    }
};