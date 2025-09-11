#pragma once
#include <cstdint>
// This header was created to bypass swig's header preprocessing.

namespace capnp {
namespace schemas {

enum class TypeEnum_f4ee4873bc65f8f0 : uint16_t {
    FILTER,
    EQ,
    DISTORTION,
    CONTROL,
    VOL,
    LOAD,
    UNLOAD,
    BPM_CONTROL,
    ECHO,
    OSC_FILTER,
    FLANGER,
    PHASER,
    TRANCE,
    PANNER,
    BATTLE_DJ,
    ROLL,
    COMPRESSOR,
    ROBOT,
};
using TypeEnum = TypeEnum_f4ee4873bc65f8f0;

enum class DetailEnum_c6c88c32e11afb23 : uint16_t {
    HIGH,
    MID,
    LOW,
    PAUSE,
    CUE,
    TRIM,
    FADER,
    TIME_STRETCH,
    SPIN,
    PITCH,
    REV,
    SCRATCH,
    BSCRATCH,
};
using DetailEnum = DetailEnum_c6c88c32e11afb23;
} // namespace schemas
} // namespace capnp

typedef capnp::schemas::TypeEnum_f4ee4873bc65f8f0   TypeEnum;
typedef capnp::schemas::DetailEnum_c6c88c32e11afb23 DetailEnum;
