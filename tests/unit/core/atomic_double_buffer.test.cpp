#include "Spinlock_Double_Buffer.hpp"
#include <doctest/doctest.h>

#include <initializer_list>
#include <stdexcept>

namespace {

struct ThrowingCopy {
    int value;
    int failure;

    explicit ThrowingCopy(int v, int f = 0) noexcept : value(v), failure(f)
    {
    }

    ThrowingCopy(const ThrowingCopy &other)
        : value(other.value), failure(other.failure)
    {
        if (failure == 1) {
            throw std::runtime_error("injected copy failure");
        }
        if (failure == 2) {
            throw 7;
        }
    }
};

} // namespace

TEST_CASE("core: atomic double buffer starts empty and returns writes in order")
{
    Spinlock_Double_Buffer<int> buf(8);

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
    Spinlock_Double_Buffer<int> buf(8);

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
    Spinlock_Double_Buffer<int> buf(16);

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

TEST_CASE("core: atomic double buffer releases spinlock after a copy exception")
{
    // Cover both reserved writes and reallocation, on both buffer sides.
    for (std::size_t reserve_size : { 0U, 2U }) {
        for (int failure : { 1, 2 }) {
            Spinlock_Double_Buffer<ThrowingCopy> buf(reserve_size);
            for (int cycle = 0; cycle < 2; ++cycle) {
                const ThrowingCopy before(cycle * 2);
                const ThrowingCopy rejected(-1, failure);
                const ThrowingCopy after(cycle * 2 + 1);
                buf.Write(before);

                if (failure == 1) {
                    CHECK_THROWS_AS(buf.Write(rejected), std::runtime_error);
                } else {
                    CHECK_THROWS_AS(buf.Write(rejected), int);
                }

                // Both operations would spin forever if Write leaked the lock.
                buf.Write(after);
                auto *batch = buf.Get();
                REQUIRE(batch != nullptr);
                REQUIRE(batch->size() == 2);
                CHECK((*batch)[0].value == before.value);
                CHECK((*batch)[1].value == after.value);
            }
            CHECK(buf.Get()->empty());
        }
    }
}
