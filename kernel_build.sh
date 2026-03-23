#!/bin/bash
OCCA_PATH=$1
INPUT_OKL="third_party/OKL/STFT_MAIN.okl"
OUTPUT_DIR="third_party/OKL/OKL_Gens"
mkdir -p $OUTPUT_DIR
#!/bin/bash


$OCCA_PATH translate -D ROOTISBASH -m opencl ./third_party/OKL/STFT_MAIN.okl \
> ./third_party/OKL/OKL_Gens/OpenCL/kernel/STFT_MAIN.cl

{
    echo "#define _USE_MATH_DEFINES";
    $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m cuda ./third_party/OKL/STFT_MAIN.okl
}> ./third_party/OKL/OKL_Gens/CUDA/kernel/STFT_MAIN.cu

{
    echo "#define _USE_MATH_DEFINES";
    $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m openmp ./third_party/OKL/STFT_MAIN.okl 
}> ./third_party/OKL/OKL_Gens/OpenMP/kernel/compiled.hpp

{
    echo "#define _USE_MATH_DEFINES";
    $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m serial ./third_party/OKL/STFT_MAIN.okl 
}> ./third_party/OKL/OKL_Gens/Serial/kernel/compiled.hpp

{
    echo "#define _USE_MATH_DEFINES";
    $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m hip ./third_party/OKL/STFT_MAIN.okl 
}> ./third_party/OKL/OKL_Gens/HIP/kernel/compiled.hpp

{
    echo "#define _USE_MATH_DEFINES";
    $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m metal ./third_party/OKL/STFT_MAIN.okl 
}> ./third_party/OKL/OKL_Gens/METAL/kernel/compiled.hpp

printf "#pragma once\nclass okl_embed {\n public:\n const char* opencl_code = \n R\"(" | cat - ./third_party/OKL/OKL_Gens/OpenCL/kernel/STFT_MAIN.cl > ./third_party/OKL/OKL_Gens/OpenCL/kernel/temp.txt


{
    cat ./third_party/OKL/OKL_Gens/OpenCL/kernel/temp.txt - <<EOF
    )";};
EOF
}> ./third_party/OKL/OKL_Gens/OpenCL/kernel/okl_embed.hpp