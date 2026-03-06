#include "PDJE_interface.hpp"
#include "util/function/image/WaveformPng.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

constexpr std::array<std::uint8_t, 8> kPngSignature {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
};

std::optional<fs::path>
find_project_root()
{
    std::error_code ec;

    fs::path probe = fs::current_path(ec);
    if (!ec) {
        for (int i = 0; i < 8; ++i) {
            if (fs::is_directory(probe / "DMCA_FREE_DEMO_MUSIC", ec)) {
                return probe;
            }
            if (!probe.has_parent_path()) {
                break;
            }
            probe = probe.parent_path();
        }
    }

    probe = fs::absolute(fs::path(__FILE__), ec).parent_path();
    if (!ec) {
        for (int i = 0; i < 8; ++i) {
            if (fs::is_directory(probe / "DMCA_FREE_DEMO_MUSIC", ec)) {
                return probe;
            }
            if (!probe.has_parent_path()) {
                break;
            }
            probe = probe.parent_path();
        }
    }

    return std::nullopt;
}

std::optional<fs::path>
pick_demo_music(const fs::path &project_root)
{
    const fs::path demo_dir = project_root / "DMCA_FREE_DEMO_MUSIC";
    const fs::path preferred = demo_dir / "miku_temp.wav";
    std::error_code ec;

    if (fs::is_regular_file(preferred, ec)) {
        return preferred;
    }

    std::vector<fs::path> candidates;
    for (const auto &entry : fs::directory_iterator(demo_dir, ec)) {
        if (ec) {
            return std::nullopt;
        }
        if (!entry.is_regular_file(ec)) {
            continue;
        }
        if (entry.path().extension() == ".wav") {
            candidates.push_back(entry.path());
        }
    }

    if (candidates.empty()) {
        return std::nullopt;
    }

    std::sort(candidates.begin(), candidates.end());
    return candidates.front();
}

bool
write_binary_file(const fs::path &path,
                  const std::vector<std::uint8_t> &bytes)
{
    std::error_code ec;
    fs::create_directories(path.parent_path(), ec);
    if (ec) {
        return false;
    }

    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    output.write(reinterpret_cast<const char *>(bytes.data()),
                 static_cast<std::streamsize>(bytes.size()));
    return output.good();
}

bool
has_png_signature(const std::vector<std::uint8_t> &bytes)
{
    if (bytes.size() < kPngSignature.size()) {
        return false;
    }

    return std::equal(
        kPngSignature.begin(), kPngSignature.end(), bytes.begin());
}

} // namespace

int
main()
{
    const auto project_root = find_project_root();
    if (!project_root.has_value()) {
        std::cerr << "failed: could not locate project root containing "
                     "DMCA_FREE_DEMO_MUSIC"
                  << std::endl;
        return 1;
    }

    const auto music_path = pick_demo_music(project_root.value());
    if (!music_path.has_value()) {
        std::cerr << "failed: no demo wav file found in "
                  << (project_root.value() / "DMCA_FREE_DEMO_MUSIC").string()
                  << std::endl;
        return 1;
    }

    const fs::path output_dir =
        fs::current_path() / "music_to_waveform_png_output";
    const fs::path db_root =
        output_dir / "music_to_waveform_png_db";

    PDJE engine(db_root.string());

    musdata source_music;
    source_music.musicPath = music_path->string();

    auto pcm = engine.GetPCMFromMusData(source_music);
    if (pcm.empty()) {
        std::cerr << "failed: could not decode pcm from "
                  << music_path->string() << std::endl;
        return 1;
    }

    constexpr std::size_t kChannelCount = 2;
    constexpr std::size_t kYPixels = 512;
    constexpr std::size_t kPcmPerPixel = 48;
    constexpr std::size_t kXPixelsPerPng = 1024;

    auto waveform = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm = pcm,
          .channel_count = kChannelCount,
          .y_pixels = kYPixels,
          .pcm_per_pixel = kPcmPerPixel,
          .x_pixels_per_png = kXPixelsPerPng,
          .compression_level = 3 });

    if (!waveform.ok()) {
        std::cerr << "failed: waveform encode error: "
                  << waveform.status().message << std::endl;
        return 1;
    }

    std::size_t file_count = 0;
    const std::string music_stem = music_path->stem().string();
    const auto &batch = waveform.value();
    for (std::size_t channel_index = 0; channel_index < batch.size();
         ++channel_index) {
        for (std::size_t png_index = 0; png_index < batch[channel_index].size();
             ++png_index) {
            const auto &png_bytes = batch[channel_index][png_index];
            if (!has_png_signature(png_bytes)) {
                std::cerr << "failed: generated bytes are not a png for channel "
                          << channel_index << " index " << png_index
                          << std::endl;
                return 1;
            }

            const fs::path output_path =
                output_dir /
                (music_stem + "_channel_" + std::to_string(channel_index) +
                 "_part_" + std::to_string(png_index) + ".png");

            if (!write_binary_file(output_path, png_bytes)) {
                std::cerr << "failed: could not write "
                          << output_path.string() << std::endl;
                return 1;
            }

            ++file_count;
        }
    }

    if (file_count == 0) {
        std::cerr << "failed: waveform encoder returned no png files"
                  << std::endl;
        return 1;
    }

    std::cout << "source: " << music_path->string() << std::endl;
    std::cout << "pcm samples: " << pcm.size() << std::endl;
    std::cout << "channels: " << batch.size() << std::endl;
    std::cout << "png files written: " << file_count << std::endl;
    std::cout << "output dir: " << output_dir.string() << std::endl;
    std::cout << "proof: first generated payload passed png signature check"
              << std::endl;

    return 0;
}
