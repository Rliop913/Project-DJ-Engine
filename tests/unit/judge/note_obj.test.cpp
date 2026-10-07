#include <doctest/doctest.h>

#include "NoteOBJ/PDJE_Note_OBJ.hpp"
#include "PDJE_Judge_Init.hpp"
#include <array>
#include <optional>
#include <utility>

using namespace PDJE_JUDGE;

TEST_CASE("judge: note object default constructs with expected defaults")
{
    NOTE n;
    CHECK(n.type.empty());
    CHECK(n.detail == 0);
    CHECK(n.first.empty());
    CHECK(n.second.empty());
    CHECK(n.third.empty());
    CHECK(n.microsecond == 0);
    CHECK_FALSE(n.used);
    CHECK(n.isDown);
}

TEST_CASE("judge: note object stores and retrieves all fields")
{
    NOTE n;
    n.type        = "tap";
    n.detail      = 42;
    n.first       = "lane1";
    n.second      = "effect";
    n.third       = "extra";
    n.microsecond = 1234567;
    n.used        = true;
    n.isDown      = false;

    CHECK(n.type == "tap");
    CHECK(n.detail == 42);
    CHECK(n.first == "lane1");
    CHECK(n.second == "effect");
    CHECK(n.third == "extra");
    CHECK(n.microsecond == 1234567);
    CHECK(n.used);
    CHECK_FALSE(n.isDown);
}

TEST_CASE("judge: obj buffer fill and get operations")
{
    OBJ obj;

    NOTE n1;
    n1.microsecond = 1000;
    n1.type        = "tap";

    NOTE n2;
    n2.microsecond = 2000;
    n2.type        = "hold";

    obj.Fill<BUFFER_MAIN>(n1, 1);
    obj.Fill<BUFFER_MAIN>(n2, 1);
    obj.Sort();

    P_NOTE_VEC found;
    obj.Get<BUFFER_MAIN>(1500, 1, found);

    REQUIRE(found.size() == 1);
    CHECK(found[0]->microsecond == 1000);
    CHECK(found[0]->type == "tap");
}

TEST_CASE("judge: obj buffer returns empty for non-existent rail")
{
    OBJ obj;

    P_NOTE_VEC found;
    obj.Get<BUFFER_MAIN>(1000, 999, found);

    CHECK(found.empty());
}

TEST_CASE("judge: obj cut removes expired notes")
{
    OBJ obj;

    NOTE n1;
    n1.microsecond = 1000;
    n1.used        = false;

    NOTE n2;
    n2.microsecond = 2000;
    n2.used        = false;

    NOTE n3;
    n3.microsecond = 3000;
    n3.used        = false;

    obj.Fill<BUFFER_MAIN>(n1, 1);
    obj.Fill<BUFFER_MAIN>(n2, 1);
    obj.Fill<BUFFER_MAIN>(n3, 1);
    obj.Sort();

    std::unordered_map<uint64_t, NOTE_VEC> cuts;
    obj.Cut<BUFFER_MAIN>(2500, cuts);

    REQUIRE(cuts.contains(1));
    CHECK(cuts[1].size() == 2);
    CHECK(cuts[1][0].microsecond == 1000);
    CHECK(cuts[1][1].microsecond == 2000);
}

TEST_CASE("judge: NOTE ResetForRestart preserves authored fields")
{
    for (const bool is_down : { false, true }) {
        NOTE note{ .type        = "hold",
                   .detail      = 42,
                   .first       = "lane1",
                   .second      = "effect",
                   .third       = "extra",
                   .microsecond = 1234567,
                   .used        = true,
                   .isDown      = is_down };
        static_assert(noexcept(note.ResetForRestart()));

        for (int repeat = 0; repeat < 2; ++repeat) {
            note.ResetForRestart();
            CHECK_FALSE(note.used);
            CHECK(note.type == "hold");
            CHECK(note.detail == 42);
            CHECK(note.first == "lane1");
            CHECK(note.second == "effect");
            CHECK(note.third == "extra");
            CHECK(note.microsecond == 1234567);
            CHECK(note.isDown == is_down);
        }
    }
}

TEST_CASE("judge: NOTE_ITR ResetForRestart handles an empty buffer")
{
    NOTE_ITR notes;
    static_assert(noexcept(notes.ResetForRestart()));
    notes.ResetForRestart();
    CHECK(notes.vec.empty());
    CHECK(notes.itr == notes.vec.begin());
    CHECK(notes.itr == notes.vec.end());
    notes.ResetForRestart();
    CHECK(notes.itr == notes.vec.end());
}

TEST_CASE("judge: NOTE_ITR ResetForRestart resets every note without "
          "reallocating or sorting")
{
    NOTE_ITR notes;
    notes.vec.resize(3);
    notes.vec[0].microsecond = 3000;
    notes.vec[1].microsecond = 1000;
    notes.vec[2].microsecond = 2000;
    notes.vec[0].used        = true;
    notes.vec[2].used        = true;
    notes.vec[2].isDown      = false;
    notes.itr                = notes.vec.end();

    SUBCASE("partially advanced cursor")
    {
        notes.itr = notes.vec.begin() + 1;
    }
    SUBCASE("exhausted cursor")
    {
    }

    const auto *storage  = notes.vec.data();
    const auto  capacity = notes.vec.capacity();
    for (int repeat = 0; repeat < 2; ++repeat) {
        notes.ResetForRestart();
        REQUIRE(notes.vec.size() == 3);
        CHECK(notes.vec.data() == storage);
        CHECK(notes.vec.capacity() == capacity);
        CHECK(notes.itr == notes.vec.begin());
        for (const auto &note : notes.vec) {
            CHECK_FALSE(note.used);
        }
        CHECK(notes.vec[0].microsecond == 3000);
        CHECK(notes.vec[1].microsecond == 1000);
        CHECK(notes.vec[2].microsecond == 2000);
        CHECK(notes.vec[0].isDown);
        CHECK_FALSE(notes.vec[2].isDown);
    }
}

namespace {
using MoveStorage = std::array<P_NOTE_VEC, 2>;

MoveStorage
PrepareMoveSource(OBJ &notes)
{
    NOTE first;
    first.microsecond = 1000;
    NOTE second;
    second.microsecond = 2000;
    for (const auto &note : { first, second }) {
        notes.Fill<BUFFER_MAIN>(note, 1);
        notes.Fill<BUFFER_SUB>(note, 1);
    }
    notes.Sort();
    MoveStorage storage;
    notes.Get<BUFFER_MAIN>(2000, 1, storage[0]);
    notes.Get<BUFFER_SUB>(2000, 1, storage[1]);
    // Advance one cursor and exhaust the other before transferring ownership.
    storage[0][0]->used = true;
    storage[1][0]->used = true;
    storage[1][1]->used = true;
    P_NOTE_VEC found;
    notes.Get<BUFFER_MAIN>(2000, 1, found);
    notes.Get<BUFFER_SUB>(2000, 1, found);
    // Make a cursor reset observable independently of the used flags.
    storage[0][0]->used = false;
    storage[1][0]->used = false;
    return storage;
}

void
CheckMovedStorage(OBJ &notes, const MoveStorage &storage)
{
    P_NOTE_VEC found;
    notes.Get<BUFFER_MAIN>(2000, 1, found);
    REQUIRE(found.size() == 1);
    CHECK(found[0] == storage[0][1]);
    notes.Get<BUFFER_SUB>(2000, 1, found);
    CHECK(found.empty());

    notes.ResetForRestart();
    notes.Get<BUFFER_MAIN>(2000, 1, found);
    CHECK(found == storage[0]);
    notes.Get<BUFFER_SUB>(2000, 1, found);
    CHECK(found == storage[1]);
    for (const auto *note : found) {
        CHECK_FALSE(note->used);
    }
}
} // namespace

TEST_CASE(
    "judge: OBJ move preserves storage and cursors after source destruction")
{
    std::optional<OBJ> source(std::in_place);
    const auto         storage = PrepareMoveSource(*source);
    std::optional<OBJ> destination;

    SUBCASE("move construction")
    {
        destination.emplace(std::move(*source));
    }
    SUBCASE("move assignment replaces existing storage")
    {
        destination.emplace();
        PrepareMoveSource(*destination);
        *destination = std::move(*source);
    }
    source.reset();
    REQUIRE(destination.has_value());
    CheckMovedStorage(*destination, storage);
}

TEST_CASE("judge: prepared init move transfers note and rail storage")
{
    std::optional<Judge_Init> prepared(std::in_place);
    prepared->note_objects.emplace();
    const auto storage = PrepareMoveSource(*prepared->note_objects);
    prepared->raildb.offset["move-port"] = 123;
    const auto *offset_address = &prepared->raildb.offset.at("move-port");
    Judge_Init  destination;

    SUBCASE("empty destination as in Gameplay Ready")
    {
    }
    SUBCASE("destination already owns notes")
    {
        destination.note_objects.emplace();
        PrepareMoveSource(*destination.note_objects);
        destination.raildb.offset["old-port"] = 456;
    }
    destination = std::move(*prepared);
    prepared.reset();
    REQUIRE(destination.note_objects.has_value());
    CheckMovedStorage(*destination.note_objects, storage);
    REQUIRE(destination.raildb.offset.size() == 1);
    CHECK(&destination.raildb.offset.at("move-port") == offset_address);
    CHECK(destination.raildb.offset.at("move-port") == 123);
}
