#pragma once

#include <string>
#include <optional>
#include <vector>

#include <miniaudio.h>
#include <filesystem>
#include "FrameCalc.hpp"
#include "fileNameSanitizer.hpp"
#include "dbRoot.hpp"

namespace fs = std::filesystem;
// using MAYBE_FRAME = std::optional<std::vector<float>>;

using FRAME_POS = unsigned long long;
/**
 * @brief miniaudio decoder wrapper class
 * 
 */
struct PDJE_API Decoder{
    ma_decoder dec;
    std::vector<uint8_t> musicBinary;
    Decoder();
    ~Decoder();
    /**
     * @brief init decoder
     * 
     * @param litedb database
     * @param KeyOrPath you can use music's path or music's key data in database
     * @return true 
     * @return false 
     */
    bool init(litedb& db, const SANITIZED_ORNOT& KeyOrPath);
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