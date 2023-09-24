#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>
#include <vector>

namespace {
std::vector<std::string> Split(const std::string& string) {
    std::vector<std::string> result;

    if (!string.empty()) {
        std::string current_string = string;
        while (true) {
            auto iterator = std::find_if(current_string.begin(), current_string.end(),
                                         [](char const& c) { return !std::isalpha(c); });

            result.push_back(current_string.substr(0, iterator - current_string.begin()));
            if (iterator == current_string.end()) {
                break;
            }

            ++iterator;
            current_string = current_string.substr(iterator - current_string.begin());
        }
    }

    return result;
}
}  // namespace

int DifferentWordsCount(const std::string& string) {
    std::string str = string;
    std::unordered_set<std::string> words;

    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    for (auto word : Split(str)) {
        if (!word.empty()) {
            words.insert(word);
        }
    }

    return words.size();
}
