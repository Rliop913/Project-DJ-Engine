#include <doctest/doctest.h>

#include "util/common/Status.hpp"
#include "util/common/StatusCode.hpp"

#include <string>

using namespace PDJE_UTIL::common;

TEST_CASE("util: status code enum contains expected values")
{
    CHECK(static_cast<int>(StatusCode::ok) == 0);
    CHECK(static_cast<int>(StatusCode::invalid_argument) == 1);
    CHECK(static_cast<int>(StatusCode::not_found) == 2);
    CHECK(static_cast<int>(StatusCode::type_mismatch) == 3);
    CHECK(static_cast<int>(StatusCode::unsupported) == 4);
    CHECK(static_cast<int>(StatusCode::io_error) == 5);
    CHECK(static_cast<int>(StatusCode::closed) == 6);
    CHECK(static_cast<int>(StatusCode::backend_error) == 7);
    CHECK(static_cast<int>(StatusCode::out_of_range) == 8);
    CHECK(static_cast<int>(StatusCode::internal_error) == 9);
}

TEST_CASE("util: status default constructs to ok")
{
    Status s;
    CHECK(s.ok());
    CHECK(s.code == StatusCode::ok);
    CHECK(s.message.empty());
}

TEST_CASE("util: status with error code reports not ok")
{
    Status s{ StatusCode::invalid_argument, "invalid input" };
    CHECK_FALSE(s.ok());
    CHECK(s.code == StatusCode::invalid_argument);
    CHECK(s.message == "invalid input");
}

TEST_CASE("util: status with ok code but non-empty message still ok")
{
    Status s{ StatusCode::ok, "informational" };
    CHECK(s.ok());
    CHECK(s.code == StatusCode::ok);
    CHECK(s.message == "informational");
}

TEST_CASE("util: status equality compares code and message")
{
    Status s1{ StatusCode::not_found, "file not found" };
    Status s2{ StatusCode::not_found, "file not found" };
    Status s3{ StatusCode::not_found, "different message" };
    Status s4{ StatusCode::invalid_argument, "file not found" };

    CHECK(s1.code == s2.code);
    CHECK(s1.message == s2.message);
    CHECK(s1.code == s3.code);
    CHECK_FALSE(s1.message == s3.message);
    CHECK_FALSE(s1.code == s4.code);
}
