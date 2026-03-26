#!/bin/bash
OCCA_PATH=$1
INPUT_OKL="./STFT_MAIN.okl"
OUTPUT_DIR="./GenOut"
TARGET_OPENCL_DIR="./GenOut/OpenCL"
TARGET_OPENMP_DIR="./GenOut/OpenMP"
TARGET_METAL_DIR="./GenOut/METAL"

mkdir -p $OUTPUT_DIR
mkdir -p $TARGET_OPENCL_DIR
mkdir -p $TARGET_OPENMP_DIR
mkdir -p $TARGET_METAL_DIR


$OCCA_PATH translate -D ROOTISBASH -m opencl $INPUT_OKL \
> ${TARGET_OPENCL_DIR}/STFT_MAIN.cl

# {
#     echo "#define _USE_MATH_DEFINES";
#     $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m cuda $INPUT_OKL
# }> ./third_party/OKL/OKL_Gens/CUDA/kernel/STFT_MAIN.cu

{
    echo "#define _USE_MATH_DEFINES";
    $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m openmp $INPUT_OKL 
}> ./${TARGET_OPENMP_DIR}/openmp_compiled.hpp

# {
#     echo "#define _USE_MATH_DEFINES";
#     $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m serial $INPUT_OKL 
# }> ./third_party/OKL/OKL_Gens/Serial/kernel/compiled.hpp

# {
#     echo "#define _USE_MATH_DEFINES";
#     $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m hip $INPUT_OKL 
# }> ./third_party/OKL/OKL_Gens/HIP/kernel/compiled.hpp

{
    echo "#define _USE_MATH_DEFINES";
    $OCCA_PATH translate -D __NEED_PI -D ROOTISBASH -m metal $INPUT_OKL 
}> ${TARGET_METAL_DIR}/metal_compiled.hpp

printf "#pragma once\nclass okl_embed {\n public:\n const char* opencl_code = \n R\"(" | cat - ${TARGET_OPENCL_DIR}/STFT_MAIN.cl > ${TARGET_OPENCL_DIR}/temp.txt


{
    cat ${TARGET_OPENCL_DIR}/temp.txt - <<EOF
    )";};
EOF
}> ${TARGET_OPENCL_DIR}/opencl_compiled.hpp

rm ${TARGET_OPENCL_DIR}/temp.txt
rm ${TARGET_OPENCL_DIR}/STFT_MAIN.cl