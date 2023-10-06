#pragma once

#include <stdexcept>
#include <vector>
#include <cstddef>

namespace {
void Swap(int* a, int* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}
}  // namespace

void Rotate(std::vector<int>* data, size_t shift) {
}
