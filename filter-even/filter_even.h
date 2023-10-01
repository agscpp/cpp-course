#pragma once

#include <cstddef>
#include <vector>

void FilterEven(std::vector<int>* data) {
    std::size_t odd_number_index = 0;
    std::size_t data_size = data->size();
    for (std::size_t index = 0; index < data_size; ++index) {
        if (data->at(index) % 2 == 0) {
            std::swap(data->at(index), data->at(odd_number_index));
            ++odd_number_index;
        }
    }
    while (odd_number_index++ < data_size) {
        data->pop_back();
    }
}
