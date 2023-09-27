#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

int64_t CountPairs(const std::vector<int>& data, int x) {
    int64_t count = 0;
    std::unordered_map<int64_t, int64_t> values;

    for (const auto value : data) {
        int64_t diff = static_cast<int64_t>(x) - static_cast<int64_t>(value);

        if (values.contains(diff)) {
            count += values[diff];
        }
        values[value] += 1;
    }

    return count;
}
