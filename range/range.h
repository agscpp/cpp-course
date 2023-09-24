#pragma once

#include <vector>

bool IsInRange(int from, int to, int value) {
    return ((from <= value && value < to) || (from >= value && value > to));
}

std::vector<int> Range(int from, int to, int step = 1) {
    std::vector<int> result;

    if ((from > to && step < 0) || (from < to && step > 0)) {
        for (int value = from;; value += step) {
            if (!IsInRange(from, to, value)) {
                break;
            }
            result.push_back(value);
        }
    }

    return result;
}
