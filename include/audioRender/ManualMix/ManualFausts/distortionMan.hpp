/**
 * @file distortionMan.hpp
 * @brief Manual distortion effect controller.
 */
#pragma once
#include "DeckData.hpp"

/**
 * @brief Manages distortion effect settings.
 */
class DistortionMan{
public:
    float distortionValue; ///< The amount of distortion to apply.

    /**
     * @brief Creates a map of argument setters for the distortion effect.
     * @return An ARGSETTER map for configuring the distortion.
     */
    ARGSETTER
    makeArgSetter(){
        return{
            {"distortionValue", [this](double value){
                this->distortionValue = static_cast<float>(value);
            }}
        };
    }
};