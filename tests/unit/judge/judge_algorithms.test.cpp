#include "Init/PDJE_Judge_Init_Structs.hpp"
#include "PDJE_RAIL.hpp"
#include <doctest/doctest.h>

#include <unordered_map>

TEST_CASE("judge: frame to microsecond conversion matches 48kHz basis")
{
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(0) == 0);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(48) == 1000);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(96) == 2000);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(6) == 125);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(12) == 250);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(48000) == 1000000);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(96000) == 2000000);
}

TEST_CASE("judge: frame to microsecond conversion uses deterministic floor policy")
{
    // Regression guard: avoid floating-point rounding drift on exact boundaries.
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(1) == 20);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(2) == 41);
    CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(3) == 62);

    uint64_t prev = PDJE_JUDGE::Convert_Frame_Into_MicroSecond(0);
    for (uint64_t frame = 1; frame <= 64; ++frame) {
        const auto cur = PDJE_JUDGE::Convert_Frame_Into_MicroSecond(frame);
        CHECK(cur >= prev);
        prev = cur;
    }

    for (uint64_t frame = 0; frame <= 64; ++frame) {
        CHECK(PDJE_JUDGE::Convert_Frame_Into_MicroSecond(frame + 6) ==
              PDJE_JUDGE::Convert_Frame_Into_MicroSecond(frame) + 125);
    }
}

TEST_CASE("judge: KB_MOUSE rail key works with unordered_map hashing")
{
    PDJE_JUDGE::RAIL_KEY::KB_MOUSE k1;
    k1.Device_Name = "kbd-main";
    k1.DeviceKey   = static_cast<BITMASK>(PDJE_KEY::A);

    PDJE_JUDGE::RAIL_KEY::KB_MOUSE k2 = k1;

    std::unordered_map<PDJE_JUDGE::RAIL_KEY::KB_MOUSE, int> m;
    m[k1] = 42;

    REQUIRE(m.find(k2) != m.end());
    CHECK(m[k2] == 42);
}

TEST_CASE("judge: MIDI rail key equality and hashing are deterministic")
{
    PDJE_JUDGE::RAIL_KEY::MIDI m1;
    m1.port_name = "midi-port";
    m1.type      = 1;
    m1.ch        = 2;
    m1.pos       = 64;

    PDJE_JUDGE::RAIL_KEY::MIDI m2 = m1;

    CHECK(m1 == m2);
    CHECK(std::hash<PDJE_JUDGE::RAIL_KEY::MIDI>{}(m1) ==
          std::hash<PDJE_JUDGE::RAIL_KEY::MIDI>{}(m2));
}
