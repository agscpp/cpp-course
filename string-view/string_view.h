#pragma once

#include <cstddef>
#include <cstring>
#include <string>

class StringView {
public:
    StringView(const std::string& str) : data_(str.c_str()), size_(str.length()) {
    }

    StringView(const std::string& str, std::size_t begin) : StringView(str.c_str() + begin) {
    }

    StringView(const std::string& str, std::size_t begin, std::size_t size)
        : StringView(str.c_str() + begin, size) {
    }

    StringView(const char* str) : data_(str), size_(std::strlen(str)) {
    }

    StringView(const char* str, std::size_t size) : data_(str), size_(size) {
    }

    const char* Data() const {
        return data_;
    }

    std::size_t Size() const {
        return size_;
    }

    const char& operator[](std::size_t offset) const {
        return *(data_ + offset);
    }

private:
    const char* data_;
    std::size_t size_;
};
