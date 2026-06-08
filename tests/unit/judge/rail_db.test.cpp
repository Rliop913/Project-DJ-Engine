#include "InputParser.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_RAIL.hpp"
#include <doctest/doctest.h>

#include <algorithm>
#include <cstring>
#include <string>
#include <variant>
#include <vector>
#include <utility>

namespace {

PDJE_JUDGE::RAIL_KEY::KB_MOUSE
MakeKeyboardKey(std::string name, PDJE_KEY key)
{
    PDJE_JUDGE::RAIL_KEY::KB_MOUSE k;
    k.Device_Name = std::move(name);
    k.DeviceKey   = static_cast<BITMASK>(key);
    return k;
}

PDJE_JUDGE::RAIL_KEY::MIDI
MakeMidiKey(std::string port, uint8_t type, uint8_t ch, uint8_t pos)
{
    PDJE_JUDGE::RAIL_KEY::MIDI k;
    k.port_name = std::move(port);
    k.type      = type;
    k.ch        = ch;
    k.pos       = pos;
    return k;
}

template <std::size_t N>
std::size_t
CopyFixed(char (&dst)[N], const std::string &value)
{
    const auto length = std::min(value.size(), N);
    std::memcpy(dst, value.data(), length);
    return length;
}

} // namespace

TEST_CASE("judge: rail identity normalization keeps byte prefixes")
{
    const std::string short_name = "short-name";
    CHECK(PDJE_JUDGE::NormalizeRailIdentity(short_name) == short_name);

    const std::string prefix(
        PDJE_JUDGE::RAIL_IDENTITY_MAX_BYTES, 'p');
    const std::string long_name = prefix + "-suffix";

    CHECK(PDJE_JUDGE::NormalizeRailIdentity(long_name) == prefix);
    CHECK(PDJE_JUDGE::NormalizeRailIdentity(long_name.data(),
                                            long_name.size()) == prefix);
}

TEST_CASE("judge: init normalizes long keyboard rail names to prefix")
{
    const std::string prefix(
        PDJE_JUDGE::RAIL_IDENTITY_MAX_BYTES, 'k');
    const std::string long_name = prefix + "-keyboard-suffix";

    PDJE_JUDGE::Judge_Init init;
    DeviceData             device {};
    device.Type               = PDJE_Dev_Type::KEYBOARD;
    device.Name               = long_name;
    device.device_specific_id = "keyboard-id";

    init.SetRail(device, static_cast<BITMASK>(PDJE_KEY::A), 123, 7);

    const auto prefix_key = MakeKeyboardKey(prefix, PDJE_KEY::A);
    REQUIRE(init.raildb.GetID(prefix_key).has_value());
    CHECK(init.raildb.GetID(prefix_key).value() == 7);
    REQUIRE(init.raildb.offset.contains(prefix));
    CHECK(init.raildb.offset[prefix] == 123);

    const auto suffix_key =
        MakeKeyboardKey(long_name.substr(PDJE_JUDGE::RAIL_IDENTITY_MAX_BYTES),
                        PDJE_KEY::A);
    CHECK_FALSE(init.raildb.GetID(suffix_key).has_value());
}

TEST_CASE("judge: init normalizes long MIDI rail names to prefix")
{
    const std::string prefix(
        PDJE_JUDGE::RAIL_IDENTITY_MAX_BYTES, 'm');
    const std::string long_name = prefix + "-midi-suffix";

    PDJE_JUDGE::Judge_Init init;
    init.SetRail(long_name, 11, 3, 4, 5, -17);

    const auto prefix_key = MakeMidiKey(prefix, 3, 4, 5);
    REQUIRE(init.raildb.GetID(prefix_key).has_value());
    CHECK(init.raildb.GetID(prefix_key).value() == 11);
    REQUIRE(init.raildb.offset.contains(prefix));
    CHECK(init.raildb.offset[prefix] == -17);

    const auto suffix_key =
        MakeMidiKey(long_name.substr(PDJE_JUDGE::RAIL_IDENTITY_MAX_BYTES),
                    3,
                    4,
                    5);
    CHECK_FALSE(init.raildb.GetID(suffix_key).has_value());
}

TEST_CASE("judge: parser applies input rail offset by normalized device name")
{
    const std::string prefix(
        PDJE_JUDGE::RAIL_IDENTITY_MAX_BYTES, 'i');
    const std::string raw_name = prefix + "x";

    PDJE_JUDGE::RAIL_DB db;
    db.Add(MakeKeyboardKey(prefix, PDJE_KEY::A),
           PDJE_Dev_Type::KEYBOARD,
           31,
           77);

    PDJE_Input_Log log {};
    log.type                    = PDJE_Dev_Type::KEYBOARD;
    log.event.keyboard.k        = PDJE_KEY::A;
    log.event.keyboard.pressed  = true;
    log.microSecond             = 1000;
    log.name_len = static_cast<uint16_t>(CopyFixed(log.name, raw_name));

    const std::string non_matching_id = "not-the-rail-name";
    log.id_len =
        static_cast<uint16_t>(CopyFixed(log.id, non_matching_id));

    PDJE_JUDGE::PARSE_OUT out {};
    PDJE_JUDGE::Parse(out, db, PDJE_JUDGE::INPUT_RAW { log });

    REQUIRE(out.logs.size() == 1);
    CHECK(out.logs.front().microSecond == 1077);
}

TEST_CASE("judge: parser applies MIDI rail offset by normalized port name")
{
    const std::string prefix(
        PDJE_JUDGE::RAIL_IDENTITY_MAX_BYTES, 'n');

    PDJE_JUDGE::RAIL_DB db;
    db.Add(MakeMidiKey(prefix, 8, 9, 10), 41, 33);

    PDJE_MIDI::MIDI_EV event {};
    event.type         = 8;
    event.ch           = 9;
    event.pos          = 10;
    event.highres_time = 2000;
    event.port_name_len =
        static_cast<uint8_t>(CopyFixed(event.port_name, prefix));

    PDJE_JUDGE::PARSE_OUT out {};
    PDJE_JUDGE::Parse(out, db, PDJE_JUDGE::MIDI_RAW { event });

    REQUIRE(out.midi_logs.size() == 1);
    CHECK(out.midi_logs.front().highres_time == 2033);
}

TEST_CASE("judge: rail db add/get/delete for keyboard paths")
{
    PDJE_JUDGE::RAIL_DB db;
    CHECK(db.Empty());

    const auto k1 = MakeKeyboardKey("kbd-main", PDJE_KEY::A);
    const auto k2 = MakeKeyboardKey("kbd-main", PDJE_KEY::B);

    db.Add(k1, PDJE_Dev_Type::KEYBOARD, 10, 123);
    CHECK_FALSE(db.Empty());
    REQUIRE(db.GetID(k1).has_value());
    CHECK(db.GetID(k1).value() == 10);
    REQUIRE(db.offset.contains("kbd-main"));
    CHECK(db.offset["kbd-main"] == 123);

    auto meta1 = db.GetMETA(10);
    REQUIRE(meta1.has_value());
    REQUIRE(meta1->size() == 1);
    REQUIRE(std::holds_alternative<PDJE_Dev_Type>(meta1->at(0).type));
    CHECK(std::get<PDJE_Dev_Type>(meta1->at(0).type) == PDJE_Dev_Type::KEYBOARD);
    REQUIRE(std::holds_alternative<PDJE_JUDGE::RAIL_KEY::KB_MOUSE>(meta1->at(0).key));
    CHECK(std::get<PDJE_JUDGE::RAIL_KEY::KB_MOUSE>(meta1->at(0).key) == k1);

    db.Add(k1, PDJE_Dev_Type::KEYBOARD, 10, 999); // duplicate should be ignored
    meta1 = db.GetMETA(10);
    REQUIRE(meta1.has_value());
    CHECK(meta1->size() == 1);
    CHECK(db.offset["kbd-main"] == 123);

    db.Add(k2, PDJE_Dev_Type::KEYBOARD, 10, 123); // same rail id, different key
    meta1 = db.GetMETA(10);
    REQUIRE(meta1.has_value());
    CHECK(meta1->size() == 2);
    REQUIRE(db.GetID(k2).has_value());

    db.Delete(k1);
    CHECK_FALSE(db.GetID(k1).has_value());
    CHECK(db.GetID(k2).has_value());
    meta1 = db.GetMETA(10);
    REQUIRE(meta1.has_value());
    CHECK(meta1->size() >= 1);
}

TEST_CASE("judge: rail db add/get/delete for midi paths")
{
    PDJE_JUDGE::RAIL_DB db;
    const auto          m1 = MakeMidiKey("midi-a", 1, 2, 64);
    const auto          m2 = MakeMidiKey("midi-b", 2, 3, 65);

    db.Add(m1, 101, -11);
    REQUIRE(db.GetID(m1).has_value());
    CHECK(db.GetID(m1).value() == 101);
    REQUIRE(db.offset.contains("midi-a"));
    CHECK(db.offset["midi-a"] == -11);

    auto meta = db.GetMETA(101);
    REQUIRE(meta.has_value());
    REQUIRE(meta->size() == 1);
    REQUIRE(std::holds_alternative<uint8_t>(meta->at(0).type));
    CHECK(std::get<uint8_t>(meta->at(0).type) == 1);
    REQUIRE(std::holds_alternative<PDJE_JUDGE::RAIL_KEY::MIDI>(meta->at(0).key));
    CHECK(std::get<PDJE_JUDGE::RAIL_KEY::MIDI>(meta->at(0).key) == m1);

    db.Add(m2, 202, 7);
    CHECK(db.GetID(m2).value() == 202);
    CHECK_FALSE(db.Empty());

    db.Delete(m1);
    CHECK_FALSE(db.GetID(m1).has_value());
    CHECK(db.GetID(m2).has_value());

    db.Clear();
    CHECK(db.Empty());
    CHECK(db.offset.empty());
    CHECK_FALSE(db.GetID(m2).has_value());
    CHECK_FALSE(db.GetMETA(202).has_value());
}
