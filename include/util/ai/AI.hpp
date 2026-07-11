#pragma once

#include "global/PDJE_EXPORT_SETTER.hpp"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace PDJE_UTIL::ai {

enum class OnnxOptimizationLevel {
    DISABLE_ALL,
    BASIC,
    EXTENDED,
    ALL,
};

struct OnnxSessionOptions {
    int                   intra_op_num_threads = 1;
    int                   inter_op_num_threads = 1;
    OnnxOptimizationLevel optimization_level   =
        OnnxOptimizationLevel::EXTENDED;

    bool
    operator==(const OnnxSessionOptions &) const = default;
};

struct FloatTensor {
    std::vector<int64_t> shape;
    std::vector<float>   values;
};

struct NamedFloatTensor {
    std::string name;
    FloatTensor tensor;
};

class PDJE_API OnnxSession {
  public:
    std::filesystem::path model_path;
    OnnxSessionOptions options;
    std::vector<std::string> input_names;
    std::vector<std::string> output_names;

    explicit OnnxSession(std::filesystem::path model_path,
                         OnnxSessionOptions    options = {});
    ~OnnxSession();

    OnnxSession(OnnxSession &&) noexcept;
    OnnxSession &
    operator=(OnnxSession &&) noexcept;

    OnnxSession(const OnnxSession &)            = delete;
    OnnxSession &operator=(const OnnxSession &) = delete;

    std::vector<NamedFloatTensor>
    run(std::span<const NamedFloatTensor> inputs) const;

    std::vector<NamedFloatTensor>
    run(std::span<const NamedFloatTensor> inputs,
        std::span<const std::string>      requested_output_names) const;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace PDJE_UTIL::ai

#include "util/ai/beat_this/BeatThis.hpp"
