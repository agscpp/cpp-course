#include <local_max.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple") {
    {
        auto data = {0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0};
        auto answer = LocalMax(data.begin(), data.end());
        REQUIRE(*answer == 5);
    }
    {
        auto data = {1};
        auto answer = LocalMax(data.begin(), data.end());
        REQUIRE(*answer == 1);
    }
    {
        auto data = {0, 1};
        auto answer = LocalMax(data.begin(), data.end());
        REQUIRE(*answer == 1);
    }
    {
        auto data = {1, 0};
        auto answer = LocalMax(data.begin(), data.end());
        REQUIRE(*answer == 1);
    }
    {
        auto data = {1, 2, 1, 5, 3};
        auto answer = LocalMax(data.begin(), data.end());
        REQUIRE(*answer == 2);
    }
}

TEST_CASE("Emtpy") {
    {
        std::vector<int> data;
        auto answer = LocalMax(data.begin(), data.end());
        REQUIRE(answer == data.end());
    }
    {
        auto data = {1, 2, 1};
        auto answer = LocalMax(data.begin(), data.begin());
        REQUIRE(answer == data.begin());
    }
}
