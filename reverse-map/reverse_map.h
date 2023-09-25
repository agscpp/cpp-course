#pragma once

#include <string>
#include <unordered_map>

std::unordered_map<int, std::string> ReverseMap(const std::unordered_map<std::string, int>& map) {
    std::unordered_map<int, std::string> result;

    for (const auto& i : map) {
        result[i.second] = i.first;
    }

    return result;
}
