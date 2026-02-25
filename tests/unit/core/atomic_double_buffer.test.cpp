#include "Atomic_Double_Buffer.hpp"
#include <doctest/doctest.h>

TEST_CASE("core: atomic double buffer starts empty and returns writes in order")
{
    Atomic_Double_Buffer<int> buf(8);

    auto *initial = buf.Get();
    REQUIRE(initial != nullptr);
    CHECK(initial->empty());

    buf.Write(1);
    buf.Write(2);
    buf.Write(3);

    auto *first_batch = buf.Get();
    REQUIRE(first_batch != nullptr);
    REQUIRE(first_batch->size() == 3);
    CHECK((*first_batch)[0] == 1);
    CHECK((*first_batch)[1] == 2);
    CHECK((*first_batch)[2] == 3);
}

TEST_CASE("core: atomic double buffer clears inactive side and avoids stale replay")
{
    Atomic_Double_Buffer<int> buf(8);

    buf.Write(10);
    auto *batch1 = buf.Get();
    REQUIRE(batch1 != nullptr);
    REQUIRE(batch1->size() == 1);
    CHECK((*batch1)[0] == 10);

    auto *empty_after_swap = buf.Get();
    REQUIRE(empty_after_swap != nullptr);
    CHECK(empty_after_swap->empty());

    buf.Write(20);
    buf.Write(21);
    auto *batch2 = buf.Get();
    REQUIRE(batch2 != nullptr);
    REQUIRE(batch2->size() == 2);
    CHECK((*batch2)[0] == 20);
    CHECK((*batch2)[1] == 21);
}

TEST_CASE("core: atomic double buffer preserves batch separation across cycles")
{
    Atomic_Double_Buffer<int> buf(16);

    for (int i = 0; i < 4; ++i) {
        buf.Write(i);
    }
    auto *batch1 = buf.Get();
    REQUIRE(batch1 != nullptr);
    REQUIRE(batch1->size() == 4);

    for (int i = 100; i < 103; ++i) {
        buf.Write(i);
    }
    auto *batch2 = buf.Get();
    REQUIRE(batch2 != nullptr);
    REQUIRE(batch2->size() == 3);
    CHECK((*batch2)[0] == 100);
    CHECK((*batch2)[1] == 101);
    CHECK((*batch2)[2] == 102);
}
