#include "PDJE_RAIL.hpp"
#include <doctest/doctest.h>

#include <variant>
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

} // namespace

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
