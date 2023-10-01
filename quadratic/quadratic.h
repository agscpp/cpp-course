#pragma once

#include <cmath>

enum class RootCount { kZero, kOne, kTwo, kInf };

struct Roots {
    RootCount count;
    double first;
    double second;
};

Roots SolveQuadratic(int a, int b, int c) {
    Roots roots;

    if (a == 0 && b == 0 && c == 0) {
        roots.count = RootCount::kInf;
        return roots;
    }

    int d = std::pow(b, 2) - 4 * a * c;
    if (d < 0) {
        roots.count = RootCount::kZero;
    } else if (d == 0) {
        if (a == 0) {
            roots.count = RootCount::kZero;
            return roots;
        }
        roots.count = RootCount::kOne;
        roots.first = -(static_cast<double>(b) / (2.0 * a));
    } else {
        if (a == 0) {
            roots.count = RootCount::kOne;
            roots.first = -(static_cast<double>(c) / b);
            return roots;
        }

        roots.count = RootCount::kTwo;
        roots.first = -(static_cast<double>(b) + std::sqrt(d)) / (2.0 * a);
        roots.second = -(static_cast<double>(b) - std::sqrt(d)) / (2.0 * a);

        if (roots.first > roots.second) {
            std::swap(roots.first, roots.second);
        }
    }

    return roots;
}
