#pragma once
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
#include <functional>
#include <unordered_map>
namespace PDJE_JUDGE {
struct EVENTS {
    /// IN, OUT
    std::function<void(std::unordered_map<uint64_t, NOTE_VEC>,
                       std::unordered_map<uint64_t, NOTE_VEC>)>
        missed_event;
    std::function<void(
        INPUT_RULE &rule, bool Pressed, bool IsLate, uint64_t diff)>
        used_event;
};
}; // namespace PDJE_JUDGE