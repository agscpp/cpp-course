#pragma once

#include <cmath>
#include <cstdint>

struct Point {
    int x, y;

    bool operator==(const Point& rhs) const {
        return this->x == rhs.x && this->y == rhs.y;
    }
};

struct Triangle {
    Point a, b, c;
};

double TriangleArea(const Point& a, const Point& b, const Point& c) {
    int64_t a_x = static_cast<int64_t>(a.x);
    int64_t a_y = static_cast<int64_t>(a.y);
    int64_t b_x = static_cast<int64_t>(b.x);
    int64_t b_y = static_cast<int64_t>(b.y);
    int64_t c_x = static_cast<int64_t>(c.x);
    int64_t c_y = static_cast<int64_t>(c.y);
    return std::abs((b_x - a_x) * (c_y - a_y) - (c_x - a_x) * (b_y - a_y)) / 2.0;
}

double IsPointInTriangle(const Triangle& triangle, const Point& point) {
    if (triangle.a == triangle.b && triangle.b == triangle.c) {
        return false;
    }
    double abp = TriangleArea(triangle.a, triangle.b, point);
    double bcp = TriangleArea(triangle.b, triangle.c, point);
    double cap = TriangleArea(triangle.c, triangle.a, point);
    double abc = TriangleArea(triangle.a, triangle.b, triangle.c);
    return abc == abp + bcp + cap;
}