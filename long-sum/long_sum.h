#pragma once

#include <string>

namespace {
int GetDigit(const std::string& str, std::size_t index) {
    return (index < str.size()) ? str[index] - '0' : 0;
}

std::string ReverseString(const std::string& str) {
    std::string result;

    for (auto i = str.rbegin(); i != str.rend(); ++i) {
        result += *i;
    }

    return result;
}
}  // namespace

std::string LongSum(const std::string& a, const std::string& b) {
    std::string res;
    std::string reverse_a = ReverseString(a);
    std::string reverse_b = ReverseString(b);

    int buffer = 0;
    std::size_t index = 0;

    while (index < reverse_a.size() || index < reverse_b.size() || buffer > 0) {
        int value = GetDigit(reverse_a, index) + GetDigit(reverse_b, index) + buffer;
        buffer = value / 10;
        res += (value % 10) + '0';
        ++index;
    }

    res = ReverseString(res);
    return res;
}
