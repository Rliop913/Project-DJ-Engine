#include "editor.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <filesystem>

namespace fs = std::filesystem;

#include <random>
bool
PDJE_Editor::AddMusicConfig(const SANITIZED   &NewMusicName,
                            const std::string &dir_name)
{
    std::random_device                          rd;
    std::mt19937                                gen(rd());
    std::uniform_int_distribution<unsigned int> randomFilename(
        std::numeric_limits<unsigned int>::min(),
        std::numeric_limits<unsigned int>::max());
    std::optional<DONT_SANITIZE> mfilename;
    for (int TRY_COUNT = 0; TRY_COUNT < 50; ++TRY_COUNT) {
        DONT_SANITIZE tempFilename = std::to_string(randomFilename(gen));
        if (!fs::exists(music_root / fs::path(tempFilename))) {
            mfilename = tempFilename;
            break;
        }
    }
    if (!mfilename.has_value()) {
        warnlog(
            "failed to make filename. this could be error or we have terrible "
            "luck. try again or fix here. from PDJE_Editor AddMusicConfig.");
        return false;
    }
    auto DataPath = music_root / fs::path(mfilename.value());
    try {
        musicHandle.emplace_back(DataPath, "musicmetadata.PDJE", name, email);
        return true;

    } catch (const std::exception &e) {
        critlog("something wrong on configure music. from PDJE_Editor "
                "AddMusicConfig. ErrException: ");
        critlog(e.what());
        return false;
    }
}