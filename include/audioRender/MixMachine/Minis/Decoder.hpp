#pragma once

#include <string>
#include <optional>
#include <vector>

#include <miniaudio.h>

#include "Structs.hpp"

// using MAYBE_FRAME = std::optional<std::vector<float>>;

struct Decoder{
    ma_decoder dec;

    Decoder();
    ~Decoder();

    bool init(const std::string& song_path);
    bool changePos(unsigned long Pos);
    bool getPos(unsigned long long& pos);
    bool getRange(unsigned long end, std::vector<float>& buffer);

};