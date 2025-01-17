#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <thread>
#include <mutex>
#include <optional>

#include "MixBinary.hpp"
#include "Mix.hpp"
#include "Bpm.hpp"


class MixTranslator{
private:
    unsigned int usable_threads = 0;
public:
    std::optional<MIX> mixs;
    std::optional<BPM> bpms;
    bool Read(const MixBinary<READ_MODE::READ_ONLY>& binary);

    MixTranslator();
    ~MixTranslator();
};