#pragma once

#include <cstddef>
#include <vector>

namespace {
void GeneratePermutations(std::size_t len, std::vector<int>* curr_permutation,
                          std::vector<std::vector<int>>* permutations, std::vector<bool>* used) {
    if (len == curr_permutation->size()) {
        permutations->push_back(*curr_permutation);
        return;
    }

    for (std::size_t index = 0; index < len; ++index) {
        if ((*used)[index] == false) {
            (*used)[index] = true;
            curr_permutation->push_back(index);
            GeneratePermutations(len, curr_permutation, permutations, used);
            curr_permutation->pop_back();
            (*used)[index] = false;
        }
    }
}
}  // namespace

std::vector<std::vector<int>> GeneratePermutations(std::size_t len) {
    std::vector<int> permutation;
    std::vector<std::vector<int>> permutations;
    std::vector<bool> used(len, false);
    GeneratePermutations(len, &permutation, &permutations, &used);
    return permutations;
}
