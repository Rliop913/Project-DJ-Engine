#include "util/ai/AI.hpp"

#include <onnxruntime_cxx_api.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace PDJE_UTIL::ai {

namespace {

Ort::Env &
GlobalOrtEnv()
{
    static Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "pdje_util_ai");
    return env;
}

GraphOptimizationLevel
ToOrtOptimizationLevel(const OnnxOptimizationLevel level)
{
    switch (level) {
    case OnnxOptimizationLevel::DISABLE_ALL:
        return GraphOptimizationLevel::ORT_DISABLE_ALL;
    case OnnxOptimizationLevel::BASIC:
        return GraphOptimizationLevel::ORT_ENABLE_BASIC;
    case OnnxOptimizationLevel::EXTENDED:
        return GraphOptimizationLevel::ORT_ENABLE_EXTENDED;
    case OnnxOptimizationLevel::ALL:
        return GraphOptimizationLevel::ORT_ENABLE_ALL;
    }

    throw std::invalid_argument("unsupported onnx optimization level");
}

void
ValidateSessionOptions(const OnnxSessionOptions &options)
{
    if (options.intra_op_num_threads < 0 || options.inter_op_num_threads < 0) {
        throw std::invalid_argument(
            "onnx session thread counts must not be negative");
    }
}

std::string
ReadIoName(Ort::Session                     &session,
           const bool                        input,
           const std::size_t                 index,
           Ort::AllocatorWithDefaultOptions &allocator)
{
    Ort::AllocatedStringPtr name =
        input ? session.GetInputNameAllocated(index, allocator)
              : session.GetOutputNameAllocated(index, allocator);
    if (!name || name.get()[0] == '\0') {
        throw std::runtime_error(input ? "onnx model input name is empty"
                                       : "onnx model output name is empty");
    }

    return std::string(name.get());
}

std::vector<std::string>
ReadIoNames(Ort::Session &session, const bool input)
{
    Ort::AllocatorWithDefaultOptions allocator;
    const std::size_t                count =
        input ? session.GetInputCount() : session.GetOutputCount();
    std::vector<std::string> names;
    names.reserve(count);

    for (std::size_t idx = 0; idx < count; ++idx) {
        names.push_back(ReadIoName(session, input, idx, allocator));
    }

    return names;
}

std::size_t
ExpectedTensorElementCount(const std::vector<int64_t> &shape)
{
    if (shape.empty()) {
        return 1u;
    }

    std::size_t elementCount = 1u;
    for (const int64_t dim : shape) {
        if (dim < 0) {
            throw std::invalid_argument(
                "onnx float tensor dimensions must not be negative");
        }

        const std::size_t unsignedDim = static_cast<std::size_t>(dim);
        if (unsignedDim == 0u) {
            return 0u;
        }

        if (elementCount >
            (std::numeric_limits<std::size_t>::max() / unsignedDim)) {
            throw std::overflow_error("onnx float tensor shape is too large");
        }
        elementCount *= unsignedDim;
    }

    return elementCount;
}

void
ValidateFloatTensor(const FloatTensor &tensor)
{
    const std::size_t expectedCount = ExpectedTensorElementCount(tensor.shape);
    if (tensor.values.size() != expectedCount) {
        throw std::invalid_argument(
            "onnx float tensor flattened storage size does not match shape");
    }
}

std::vector<std::string>
ResolveOutputNames(const std::vector<std::string> &knownNames,
                   std::span<const std::string>    requestedNames)
{
    if (requestedNames.empty()) {
        return knownNames;
    }

    std::vector<std::string> resolved;
    resolved.reserve(requestedNames.size());

    for (const std::string &requestedName : requestedNames) {
        if (requestedName.empty()) {
            throw std::invalid_argument(
                "onnx requested output name must not be empty");
        }

        if (std::find(knownNames.begin(), knownNames.end(), requestedName) ==
            knownNames.end()) {
            throw std::invalid_argument(
                "onnx requested output name is not recognized: " +
                requestedName);
        }

        if (std::find(resolved.begin(), resolved.end(), requestedName) !=
            resolved.end()) {
            throw std::invalid_argument(
                "onnx requested output name was duplicated: " + requestedName);
        }

        resolved.push_back(requestedName);
    }

    return resolved;
}

std::vector<const char *>
BuildNamePointers(const std::vector<std::string> &names)
{
    std::vector<const char *> rawNames;
    rawNames.reserve(names.size());

    for (const std::string &name : names) {
        rawNames.push_back(name.c_str());
    }

    return rawNames;
}

const NamedFloatTensor &
FindInputByName(std::span<const NamedFloatTensor> inputs,
                const std::string                &name)
{
    const NamedFloatTensor *found = nullptr;

    for (const NamedFloatTensor &input : inputs) {
        if (input.name != name) {
            continue;
        }

        if (found != nullptr) {
            throw std::invalid_argument("onnx input name was duplicated: " +
                                        name);
        }

        found = &input;
    }

    if (found == nullptr) {
        throw std::invalid_argument("onnx input is missing: " + name);
    }

    return *found;
}

std::vector<Ort::Value>
BuildInputValues(std::span<const NamedFloatTensor> inputs,
                 const std::vector<std::string>   &expectedNames)
{
    if (inputs.size() != expectedNames.size()) {
        throw std::invalid_argument(
            "onnx input count does not match model input count");
    }

    static const Ort::MemoryInfo memoryInfo =
        Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    std::vector<Ort::Value> values;
    values.reserve(expectedNames.size());

    for (const std::string &expectedName : expectedNames) {
        const NamedFloatTensor &input = FindInputByName(inputs, expectedName);
        if (input.name.empty()) {
            throw std::invalid_argument("onnx input name must not be empty");
        }

        ValidateFloatTensor(input.tensor);
        values.push_back(Ort::Value::CreateTensor<float>(
            memoryInfo,
            const_cast<float *>(input.tensor.values.data()),
            input.tensor.values.size(),
            input.tensor.shape.data(),
            input.tensor.shape.size()));
    }

    return values;
}

FloatTensor
CopyOutputTensor(const Ort::Value &value)
{
    if (!value.IsTensor()) {
        throw std::runtime_error("onnx output value is not a tensor");
    }

    const Ort::TensorTypeAndShapeInfo info = value.GetTensorTypeAndShapeInfo();
    if (info.GetElementType() != ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
        throw std::runtime_error("onnx output tensor is not float");
    }

    const std::vector<int64_t> shape        = info.GetShape();
    const std::size_t          elementCount = info.GetElementCount();
    const float               *data         = value.GetTensorData<float>();

    return FloatTensor{
        .shape  = shape,
        .values = std::vector<float>(data, data + elementCount),
    };
}

} // namespace

class OnnxSession::Impl {
  public:
    Impl(const std::filesystem::path &modelPath,
         const OnnxSessionOptions    &options,
         std::vector<std::string>    &inputNames,
         std::vector<std::string>    &outputNames)
    {
        if (modelPath.empty()) {
            throw std::invalid_argument(
                "onnx session model path must not be empty");
        }
        if (!std::filesystem::exists(modelPath)) {
            throw std::runtime_error("onnx model was not found: " +
                                     modelPath.string());
        }

        ValidateSessionOptions(options);

        Ort::SessionOptions sessionOptions;
        sessionOptions.SetGraphOptimizationLevel(
            ToOrtOptimizationLevel(options.optimization_level));
        if (options.intra_op_num_threads > 0) {
            sessionOptions.SetIntraOpNumThreads(options.intra_op_num_threads);
        }
        if (options.inter_op_num_threads > 0) {
            sessionOptions.SetInterOpNumThreads(options.inter_op_num_threads);
        }

        session_ = std::make_unique<Ort::Session>(
            GlobalOrtEnv(), modelPath.c_str(), sessionOptions);
        inputNames  = ReadIoNames(*session_, true);
        outputNames = ReadIoNames(*session_, false);
    }

    std::vector<NamedFloatTensor>
    run(std::span<const NamedFloatTensor> inputs,
        std::span<const std::string>      requestedOutputNames,
        const std::vector<std::string>   &inputNames,
        const std::vector<std::string>   &outputNames) const
    {
        const std::vector<std::string> resolvedOutputNames =
            ResolveOutputNames(outputNames, requestedOutputNames);
        std::vector<Ort::Value> inputValues =
            BuildInputValues(inputs, inputNames);
        const std::vector<const char *> inputNamePointers =
            BuildNamePointers(inputNames);
        const std::vector<const char *> outputNamePointers =
            BuildNamePointers(resolvedOutputNames);

        auto outputs = session_->Run(Ort::RunOptions{ nullptr },
                                     inputNamePointers.data(),
                                     inputValues.data(),
                                     inputValues.size(),
                                     outputNamePointers.data(),
                                     outputNamePointers.size());
        if (outputs.size() != resolvedOutputNames.size()) {
            throw std::runtime_error(
                "onnx runtime returned an unexpected number of outputs");
        }

        std::vector<NamedFloatTensor> results;
        results.reserve(outputs.size());
        for (std::size_t idx = 0; idx < outputs.size(); ++idx) {
            results.push_back(NamedFloatTensor{
                .name   = resolvedOutputNames[idx],
                .tensor = CopyOutputTensor(outputs[idx]),
            });
        }

        return results;
    }

    std::unique_ptr<Ort::Session> session_;
};

struct OnnxSession::Build {
    std::filesystem::path    model_path;
    OnnxSessionOptions       options;
    std::vector<std::string> input_names;
    std::vector<std::string> output_names;
    std::unique_ptr<Impl>    impl;
};

OnnxSession::Build
OnnxSession::build(std::filesystem::path model_path, OnnxSessionOptions options)
{
    Build result{ .model_path = std::move(model_path),
                  .options    = std::move(options) };
    result.impl = std::make_unique<Impl>(result.model_path,
                                         result.options,
                                         result.input_names,
                                         result.output_names);
    return result;
}

OnnxSession::OnnxSession(Build build)
    : model_path(std::move(build.model_path)),
      options(std::move(build.options)),
      input_names(std::move(build.input_names)),
      output_names(std::move(build.output_names)), impl_(std::move(build.impl))
{
}

OnnxSession::OnnxSession(std::filesystem::path model_path,
                         OnnxSessionOptions    options)
    : OnnxSession(build(std::move(model_path), std::move(options)))
{
}

OnnxSession::~OnnxSession() = default;

OnnxSession::OnnxSession(OnnxSession &&other)
    : model_path(other.model_path), options(other.options),
      input_names(other.input_names), output_names(other.output_names),
      impl_(std::move(other.impl_))
{
}

std::vector<NamedFloatTensor>
OnnxSession::run(const std::span<const NamedFloatTensor> inputs) const
{
    return run(inputs, std::span<const std::string>{});
}

std::vector<NamedFloatTensor>
OnnxSession::run(
    const std::span<const NamedFloatTensor> inputs,
    const std::span<const std::string>      requested_output_names) const
{
    if (!impl_) {
        throw std::runtime_error("onnx session is not initialized");
    }

    return impl_->run(
        inputs, requested_output_names, input_names, output_names);
}

} // namespace PDJE_UTIL::ai
