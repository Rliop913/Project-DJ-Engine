#include <doctest/doctest.h>

#include "NoteOBJ/PDJE_Note_OBJ.hpp"

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
