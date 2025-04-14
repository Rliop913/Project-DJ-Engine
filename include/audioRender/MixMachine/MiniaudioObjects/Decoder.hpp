#pragma once

#include <string>
#include <optional>
#include <vector>

#include <miniaudio.h>

#include "FrameCalc.hpp"

// using MAYBE_FRAME = std::optional<std::vector<float>>;

using FRAME_POS = unsigned long long;
/**
 * @brief miniaudio decoder wrapper class
 * 
 */
struct Decoder{
    ma_decoder dec;

    Decoder();
    ~Decoder();
    /**
     * @brief init decoder
     * 
     * @param song_path music path
     * @param root_path RootDB path
     * @return true 
     * @return false 
     */
    bool init(const std::string& song_path, const std::string& root_path);
    /**
     * @brief changes the playback position
     * 
     * @param Pos new position
     * @return true 
     * @return false 
     */
    bool changePos(FRAME_POS Pos);

    /**
     * @brief Get the playback position
     * 
     * @param pos position to be returned
     * @return true 
     * @return false 
     */
    bool getPos(FRAME_POS& pos);

    /**
     * @brief check the music's range and return decoded pcm frames
     * 
     * @param numFrames amount of frames
     * @param buffer pcm frames to be returned
     * @return true 
     * @return false 
     */
    bool getRange(FRAME_POS numFrames, std::vector<float>& buffer);

};