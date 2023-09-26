#include <long_sum.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple") {
    CHECK(LongSum("2", "2") == "4");
    CHECK(LongSum("1", "9") == "10");
    CHECK(LongSum("9", "9") == "18");
}

TEST_CASE("Empty") {
    CHECK(LongSum("", "").empty());
    CHECK(LongSum("132", "") == "132");
    CHECK(LongSum("", "111") == "111");
}

TEST_CASE("Big") {
    CHECK(LongSum("123456789", "123456789") == "246913578");
    CHECK(LongSum("999999999", "999999999") == "1999999998");
    CHECK(LongSum("9000000000", "1000000000000000000000000001000000000") ==
          "1000000000000000000000000010000000000");
}
