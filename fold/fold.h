#pragma once

#include <cstddef>
#include <vector>

struct Sum {
    template <class T>
    void operator()(T* a, const T& b) const {
        *a += b;
    }
};

struct Prod {
    template <class T>
    void operator()(T* a, const T& b) const {
        *a *= b;
    }
};

struct Concat {
    template <class T>
    void operator()(std::vector<T>* a, const std::vector<T>& b) const {
        a->insert(a->end(), b.begin(), b.end());
    }
};

class Length {
private:
    size_t* value_;

public:
    Length() = delete;

    Length(size_t* value) : value_(value) {
    }

    template <class T>
    void operator()(T*, const T&) {
        ++(*value_);
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    while (first != last) {
        func(&init, *first++);
    }
    return init;
}
