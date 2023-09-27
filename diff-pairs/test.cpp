#include <diff_pairs.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Small") {
    CHECK(CountPairs({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10) == 4);
    CHECK(CountPairs({3, 3, 3}, 3) == 0);
    CHECK(CountPairs({5, 3, 0}, 3) == 1);
    CHECK(CountPairs({5, 3, 3, 3, 0}, 3) == 3);
}

TEST_CASE("Empty") {
    CHECK(CountPairs({}, 0) == 0);
    CHECK(CountPairs({}, 5) == 0);
}