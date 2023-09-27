#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

int64_t CountPairs(const std::vector<int>& data, int x) {
    int64_t count = 0;
    std::unordered_map<int, int> values;

    for (const auto value : data) {
        if (values.contains(x - value)) {
            count += values[x - value];
        }
        values[value] += 1;
    }

    return count;
}
