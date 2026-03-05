
.. _program_listing_file_include_util_Halide_Codes_Halide_CodeSample.cpp:

Program Listing for File Halide_CodeSample.cpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_Halide_Codes_Halide_CodeSample.cpp>` (``include\util\Halide_Codes\Halide_CodeSample.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // halide_targetcode/sample.cpp
   #include "Halide.h"
   
   using namespace Halide;
   
   class SamplePipeline : public Generator<SamplePipeline> {
     public:
       // Input: 8-bit grayscale image
       Input<Buffer<uint8_t>> input{ "input", 2 };
   
       // Optional params (can be set via PARAMS in add_halide_library)
       GeneratorParam<int>  gain{ "gain", 1 }; // multiply factor
       GeneratorParam<int>  bias{ "bias", 1 }; // add bias
       GeneratorParam<bool> do_clamp{ "do_clamp", true };
   
       // Output: 8-bit grayscale image
       Output<Buffer<uint8_t>> output{ "output", 2 };
   
       // Vars
       Var  x{ "x" }, y{ "y" };
       Func f{ "f" };
   
       void
       generate()
       {
           // Simple pixel transform: out = clamp(in*gain + bias)
           Expr v = cast<int>(input(x, y)) * gain + bias;
   
           if (do_clamp) {
               v = clamp(v, 0, 255);
           }
   
           f(x, y)      = cast<uint8_t>(v);
           output(x, y) = f(x, y);
       }
   
       void
       schedule()
       {
           // Provide bounds estimates for autoschedulers (optional but helpful)
           if (using_autoscheduler()) {
               input.set_estimates({ { 0, 1920 }, { 0, 1080 } });
               output.set_estimates({ { 0, 1920 }, { 0, 1080 } });
               return;
           }
   
           // Manual schedule (used if AUTOSCHEDULER is not enabled)
           const Target t = get_target();
   
           // Some reasonable defaults
           Var xo{ "xo" }, yo{ "yo" }, xi{ "xi" }, yi{ "yi" };
   
           // GPU path: works for cuda/opencl/metal targets (if enabled via
           // FEATURES)
           if (t.has_gpu_feature()) {
               // 16x16 tiles as a sane default
               f.gpu_tile(x, y, xo, yo, xi, yi, 16, 16);
               output.compute_root();
           } else {
               // CPU path: tile + vectorize + parallel
               f.compute_root()
                   .tile(x, y, xo, yo, xi, yi, 128, 32)
                   .vectorize(xi, 16)
                   .parallel(yo);
           }
   
           // Avoid overly conservative bound checks where safe
           output.bound(x, 0, input.dim(0).extent())
               .bound(y, 0, input.dim(1).extent());
       }
   };
   
   // Name used by CMake/command line to select this generator.
   // If you pass GENERATOR SamplePipeline in CMake, it refers to this string.
   HALIDE_REGISTER_GENERATOR(SamplePipeline, samplepipe)
