#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
public:
    explicit RingBuffer(std::size_t capacity)
        : capacity_(capacity), write_index_(0), read_index_(0), size_(0) {
        elements_.resize(capacity, 0);
    }

    size_t Size() const {
        return size_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    bool TryPush(int element) {
        if (Size() == capacity_) {
            return false;
        }

        ++size_;
        write_index_ = (write_index_ < capacity_) ? write_index_ : 0;
        elements_[write_index_++] = element;
        return true;
    }

    bool TryPop(int* element) {
        if (Empty()) {
            return false;
        }

        --size_;
        read_index_ = (read_index_ < capacity_) ? read_index_ : 0;
        *element = elements_[read_index_++];
        return true;
    }

private:
    std::size_t capacity_;
    std::size_t write_index_;
    std::size_t read_index_;
    std::size_t size_;
    std::vector<int> elements_;
};
