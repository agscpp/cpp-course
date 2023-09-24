#pragma once

#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    std::vector<std::string> result;

    if (!string.empty()) {
        std::string current_string = string;
        while (true) {
            std::size_t position = current_string.find(delimiter);
            result.push_back(current_string.substr(0, position));

            if (position == std::string::npos) {
                break;
            }

            current_string = current_string.substr(position + delimiter.size());
        }
    }

    return result;
}
