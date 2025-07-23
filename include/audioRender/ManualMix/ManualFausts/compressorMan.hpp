/**
 * @file compressorMan.hpp
 * @brief Manual compressor effect controller.
 */
#pragma once
#include "DeckData.hpp"

/**
 * @brief Manages compressor effect settings.
 */
class CompressorMan{
public:
    float strength; ///< The compression ratio.
    int threshDB;   ///< The threshold in decibels.
    int attackMS;   ///< The attack time in milliseconds.
    int releaseMS;  ///< The release time in milliseconds.
    int kneeDB;     ///< The knee width in decibels.

    /**
     * @brief Creates a map of argument setters for the compressor effect.
     * @return An ARGSETTER map for configuring the compressor.
     */
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