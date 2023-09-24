#pragma once

#include <vector>

std::vector<int> Unique(const std::vector<int>& data) {
    std::vector<int> result;
    std::size_t size = data.size();

    if (size > 0) {
        result.push_back(data.front());
        for (std::size_t index = 1; index < size; ++index) {
            if (data[index] != data[index - 1]) {
                result.push_back(data[index]);
            }
        }
    }

    return result;
}
