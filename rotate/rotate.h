#pragma once

#include <vector>

namespace {
void Swap(int* a, int* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void Reverse(std::size_t left, std::size_t right, std::vector<int>* data) {
    while (left < right) {
        Swap(&(data->at(left++)), &(data->at(right--)));
    }
}
}  // namespace

void Rotate(std::vector<int>* data, std::size_t shift) {
    if (shift == 0 || data->empty() || data->size() == shift) {
        return;
    }
    Reverse(0, shift - 1, data);
    Reverse(shift, data->size() - 1, data);
    Reverse(0, data->size() - 1, data);
}
