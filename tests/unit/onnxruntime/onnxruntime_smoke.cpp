#include <onnxruntime_cxx_api.h>

#include <exception>
#include <iostream>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kExpectedOrtVersion = "1.25.0";

} // namespace

int
main()
{
    try {
        const char *c_version = OrtGetApiBase()->GetVersionString();
        if (c_version == nullptr) {
            std::cerr << "ONNX Runtime returned a null version string" << std::endl;
            return 2;
        }

        const std::string_view c_version_view(c_version);
        if (c_version_view != kExpectedOrtVersion) {
            std::cerr << "Unexpected ONNX Runtime C API version: " << c_version_view
                      << std::endl;
            return 3;
        }

        const std::string cpp_version = Ort::GetVersionString();
        if (cpp_version != kExpectedOrtVersion) {
            std::cerr << "Unexpected ONNX Runtime C++ API version: " << cpp_version
                      << std::endl;
            return 4;
        }

        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "pdje_onnxruntime_smoke");
        Ort::SessionOptions session_options;
        session_options.SetIntraOpNumThreads(1);
        session_options.SetInterOpNumThreads(1);
        session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);

        return 0;
    } catch (const Ort::Exception &ex) {
        std::cerr << "ONNX Runtime exception: " << ex.what() << std::endl;
        return 5;
    } catch (const std::exception &ex) {
        std::cerr << "std::exception: " << ex.what() << std::endl;
        return 6;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 7;
    }
}
