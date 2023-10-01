#include <point_triangle.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple") {
    Triangle triangle{Point{2, -17}, Point{5, -5}, Point{-5, -10}};

    CHECK(IsPointInTriangle(triangle, {-3, -10}));
    CHECK_FALSE(IsPointInTriangle(triangle, {-3, 10}));
    CHECK_FALSE(IsPointInTriangle(triangle, {2, 5}));
    CHECK_FALSE(IsPointInTriangle(triangle, {-2, -5}));
}

TEST_CASE("Zero") {
    Triangle triangle{Point{1, -1}, Point{1, -1}, Point{1, -1}};
    CHECK_FALSE(IsPointInTriangle(triangle, {-1, -1}));
}
