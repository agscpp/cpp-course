#pragma once

#include <stack>
#include <string>
#include <vector>

namespace {
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

bool IsMathSymbol(const std::string& str) {
    return str == "*" || str == "/" || str == "+" || str == "-";
}
}  // namespace

int EvaluateExpression(const std::string& expression) {
    std::vector<std::string> elements = Split(expression);
    std::stack<int> numbers;

    for (const auto& element : elements) {
        if (!IsMathSymbol(element)) {
            numbers.push(std::stoi(element));
            continue;
        }

        int a = numbers.top();
        numbers.pop();

        int b = numbers.top();
        numbers.pop();

        if (element == "*") {
            b *= a;
        } else if (element == "/") {
            b /= a;
        } else if (element == "+") {
            b += a;
        } else if (element == "-") {
            b -= a;
        }

        numbers.push(b);
    }

    return !numbers.empty() ? numbers.top() : 0;
}
