#pragma once

#include <cstddef>
#include <string>

class WeakPtr;

struct Block {
    size_t strong;
    size_t weak;
};

class SharedPtr {
    friend WeakPtr;

public:
    SharedPtr() = default;
    SharedPtr(const WeakPtr& ptr);

    SharedPtr(const SharedPtr& other) : value_(other.value_), counter_(other.counter_) {
        if (counter_) {
            ++counter_->strong;
        }
    }

    SharedPtr(SharedPtr&& other) : value_(other.value_), counter_(other.counter_) {
        other.value_ = nullptr;
        other.counter_ = nullptr;
    }

    SharedPtr(std::string* value) : value_(value) {
        if (value_) {
            counter_ = new Block{1, 0};
        }
    }

    ~SharedPtr() {
        if (counter_ && --counter_->strong == 0) {
            if (counter_->weak == 0) {
                delete counter_;
            }
            delete value_;
        }
    }

    SharedPtr& operator=(SharedPtr other) {
        if (other.value_ != this->value_) {
            std::swap(value_, other.value_);
            std::swap(counter_, other.counter_);
        }
        return *this;
    }

    std::string& operator*() const {
        return *value_;
    }

    std::string* operator->() const {
        return value_;
    }

    std::string* Get() const {
        return value_;
    }

    void Reset(std::string* value) {
        auto new_ptr = SharedPtr(value);
        std::swap(value_, new_ptr.value_);
        std::swap(counter_, new_ptr.counter_);
    }

private:
    std::string* value_ = nullptr;
    Block* counter_ = nullptr;
};

class WeakPtr {
    friend SharedPtr;

public:
    WeakPtr() = default;
    WeakPtr(const SharedPtr& ptr) : value_(ptr.value_), counter_(ptr.counter_) {
        if (counter_) {
            ++counter_->weak;
        }
    }

    WeakPtr(const WeakPtr& other) : value_(other.value_), counter_(other.counter_) {
        if (counter_) {
            ++counter_->weak;
        }
    }

    ~WeakPtr() {
        if (counter_ && --counter_->weak == 0 && counter_->strong == 0) {
            delete counter_;
        }
    }

    WeakPtr& operator=(WeakPtr other) {
        if (value_ != other.value_) {
            std::swap(value_, other.value_);
            std::swap(counter_, other.counter_);
        }
        return *this;
    }

    std::string& operator*() {
        return *value_;
    }

    std::string* operator->() {
        return value_;
    }

    bool IsExpired() const {
        return !counter_ || counter_->strong == 0;
    }

    SharedPtr Lock() const {
        return SharedPtr(*this);
    }

private:
    std::string* value_ = nullptr;
    Block* counter_ = nullptr;
};

SharedPtr::SharedPtr(const WeakPtr& ptr) {
    if (!ptr.IsExpired()) {
        value_ = ptr.value_;
        counter_ = ptr.counter_;
        ++counter_->strong;
    }
}
