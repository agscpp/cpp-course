#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <class T>
std::vector<std::unique_ptr<T>> Duplicate(const std::vector<std::shared_ptr<T>>& items) {
    std::vector<std::unique_ptr<T>> answer;
    for (size_t i = 0; i < items.size(); ++i) {
        auto value = *(items[i]);
        answer.push_back(std::make_unique<T>(value));
    }
    return answer;
}

template <class T>
std::vector<std::shared_ptr<T>> DeDuplicate(const std::vector<std::unique_ptr<T>>& items) {
    std::vector<std::shared_ptr<T>> answer;
    std::unordered_map<T, size_t> values;
    for (size_t i = 0; i < items.size(); ++i) {
        auto value = *(items[i]);
        if (values.contains(value)) {
            answer.push_back(answer[values[value]]);
        } else {
            values.insert({value, answer.size()});
            answer.push_back(std::make_shared<T>(value));
        }
    }
    return answer;
}
