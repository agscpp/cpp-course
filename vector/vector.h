#pragma once

#include <compare>
#include <cstdint>
#include <iterator>
#include <cstddef>

class Vector {
public:
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = int;
        using difference_type = ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        Iterator() : Iterator(nullptr) {
        }

        Iterator(pointer value) : value_(value) {
        }

        pointer operator->() const {
            return value_;
        }

        reference operator*() const {
            return *value_;
        }

        reference operator[](difference_type index) const {
            return *(value_ + index);
        }

        Iterator& operator++() {
            ++value_;
            return *this;
        }

        Iterator& operator--() {
            --value_;
            return *this;
        }

        Iterator operator++(value_type) {
            return Iterator(value_++);
        }

        Iterator operator--(value_type) {
            return Iterator(value_--);
        }

        Iterator& operator+=(difference_type offset) {
            value_ += offset;
            return *this;
        }

        Iterator& operator-=(difference_type offset) {
            value_ -= offset;
            return *this;
        }

        Iterator operator+(difference_type offset) const {
            return Iterator(value_ + offset);
        }

        Iterator operator-(difference_type offset) const {
            return Iterator(value_ - offset);
        }

        difference_type operator-(const Iterator& other) const {
            return value_ - other.value_;
        }

        friend Iterator operator+(difference_type offset, Iterator iterator) {
            return Iterator(iterator.value_ + offset);
        }

        friend Iterator operator-(difference_type offset, Iterator iterator) {
            return Iterator(iterator.value_ - offset);
        }

        std::strong_ordering operator<=>(const Iterator& other) const = default;

    private:
        pointer value_;
    };

    Vector() : size_(0), capacity_(size_), values_(nullptr) {
    }

    Vector(size_t size) : size_(size), capacity_(size_), values_(new int[capacity_]) {
        for (size_t i = 0; i < size_; ++i) {
            values_[i] = 0;
        }
    }

    Vector(std::initializer_list<int> list) : Vector(list.size()) {
        size_t index = 0;
        for (auto value : list) {
            values_[index++] = value;
        }
    }

    Vector(Vector&& other)
        : size_(other.size_), capacity_(other.capacity_), values_(other.values_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.values_ = nullptr;
    }

    Vector(const Vector& other)
        : size_(other.size_), capacity_(other.capacity_), values_(new int[capacity_]) {
        for (size_t i = 0; i < other.size_; ++i) {
            values_[i] = other.values_[i];
        }
    }

    ~Vector() {
        delete[] values_;
    }

    Vector& operator=(Vector&& other) {
        if (this == &other) {
            return *this;
        }
        Swap(other);
        return *this;
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        this->~Vector();
        new (this) Vector(other);
        return *this;
    }

    int& operator[](size_t index) {
        return values_[index];
    }

    int operator[](size_t index) const {
        return values_[index];
    }

    bool operator==(const Vector& other) const {
        if (size_ != other.size_) {
            return false;
        }

        for (size_t i = 0; i < size_; ++i) {
            if (values_[i] != other.values_[i]) {
                return false;
            }
        }

        return true;
    }

    Iterator begin() const {
        return values_;
    }

    Iterator end() const {
        return values_ + size_;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(int value) {
        if (capacity_ == size_) {
            Reserve(capacity_ > 0 ? capacity_ * 2 : 1);
        }
        values_[size_++] = value;
    }

    void PopBack() {
        if (size_ == 0) {
            return;
        }
        --size_;
    }

    void Reserve(size_t n) {
        if (n <= capacity_) {
            return;
        }

        auto new_array = new int[n];
        for (size_t i = 0; i < size_; ++i) {
            new_array[i] = values_[i];
        }

        delete[] values_;
        values_ = new_array;
        capacity_ = n;
    }

    void Clear() {
        size_ = 0;
    }

    void Swap(Vector& other) {
        auto other_size = other.size_;
        other.size_ = size_;
        size_ = other_size;

        auto other_capacity = other.capacity_;
        other.capacity_ = capacity_;
        capacity_ = other_capacity;

        auto other_values = other.values_;
        other.values_ = values_;
        values_ = other_values;
    }

private:
    size_t size_;
    size_t capacity_;
    int* values_;
};
