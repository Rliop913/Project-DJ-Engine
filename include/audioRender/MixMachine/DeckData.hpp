/**
 * @file DeckData.hpp
 * @brief Defines core data structures and type aliases used in the audio mixing process.
 */
#pragma once

#include <vector>
#include <string>
#include <functional>
#include "PDJE_EXPORT_SETTER.hpp"

/// @brief Type alias for a standard string.
using STR = std::string;

/// @brief Type alias for a function that sets a double value.
using SETTER = std::function<void(double)>;

/// @brief Type alias for a map of string keys to setter functions.
using ARGSETTER = std::unordered_map<std::string, SETTER>;

/**
 * @brief Represents a time range using start and end frames.
 */
struct Position{
    unsigned long frame_in;  ///< The starting frame of the range.
    unsigned long frame_out; ///< The ending frame of the range.
};

/**
 * @brief A mock class for Faust metadata compatibility.
 * This class provides a `declare` method to mimic Faust's metadata declaration,
 * allowing the engine to interface with Faust-generated code without modification.
 */
class Meta{
public:
    /**
     * @brief A mock declaration function for Faust compatibility.
     * @param key The metadata key (unused).
     * @param value The metadata value (unused).
     */
    void declare(std::string, std::string){
        ;//nothing. this is for faust
    }
};

/**
 * @brief A mock class for Faust UI compatibility.
 * This class provides methods to mimic Faust's UI object, allowing the engine
 * to use Faust-generated code that includes UI elements.
 */
class UI{
public:
    /**
     * @brief A mock function to open a vertical box in a Faust UI.
     * @param name The name of the box (unused).
     */
    void openVerticalBox(std::string){
        ;//nothing. this is for faust
    }
    /**
     * @brief A mock function to close a box in a Faust UI.
     */
    void closeBox(){
        ;//nothing. this is for faust
    }
};