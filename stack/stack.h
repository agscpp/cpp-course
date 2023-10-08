#pragma once

#include <cstddef>
#include <list>

class Stack {
public:
    void Push(int x) {
        values_.push_back(x);
    }

    bool Pop() {
        if (Empty()) {
            return false;
        }
        values_.pop_back();
        return true;
    }

    int Top() const {
        return !Empty() ? values_.back() : 0;
    }

    bool Empty() const {
        return values_.empty();
    }

    size_t Size() const {
        return values_.size();
    }

private:
    std::list<int> values_;
};
