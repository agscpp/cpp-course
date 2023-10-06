#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <stdexcept>

std::vector<std::pair<int64_t, int>> Factorize(int64_t value) {
    std::vector<std::pair<int64_t, int>> result;

    for (int64_t i = 2; i * i <= value; ++i) {
        int count = 0;

        while (value % i == 0) {
            value /= i;
            ++count;
        }

        if (count > 0) {
            result.push_back({i, count});
        }
    }

    if (value > 1) {
        result.push_back({value, 1});
    }

    return result;
}
