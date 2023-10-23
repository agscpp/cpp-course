#pragma once

#include <string>
#include <cstddef>

struct State {
    size_t count;
    std::string* values;
    size_t size;
    size_t capacity;
};

class COWVector {
public:
    COWVector() : state_(new State{1, nullptr, 0, 0}) {
    }

    COWVector(const COWVector& other) : state_(other.state_) {
        ++state_->count;
    }

    ~COWVector() {
        FreeState();
    }

    COWVector& operator=(const COWVector& other) {
        if (this == &other) {
            return *this;
        }
        this->~COWVector();
        new (this) COWVector(other);
        return *this;
    }

    size_t Size() const {
        return state_->size;
    }

    void Resize(size_t size) {
        if (size == state_->size || (size <= state_->capacity && state_->count == 1)) {
            state_->size = size;
            return;
        }

        std::string* new_array = nullptr;
        if (size > 0) {
            new_array = new std::string[size * 2];
            for (size_t i = 0; i < state_->size; ++i) {
                new_array[i] = state_->values[i];
            }
        }

        FreeState();
        state_ = new State{1, new_array, size, size * 2};
    }

    const std::string& Get(size_t at) const {
        return state_->values[at];
    }

    const std::string& Back() const {
        return state_->values[state_->size - 1];
    }

    void PushBack(const std::string& value) {
        Resize(state_->size + 1);
        state_->values[state_->size - 1] = value;
    }

    void Set(size_t at, const std::string& value) {
        if (state_->values[at] == value || state_->count == 1) {
            state_->values[at] = value;
            return;
        }

        auto size = state_->size;
        auto new_array = new std::string[size];

        for (size_t i = 0; i < size; ++i) {
            new_array[i] = state_->values[i];
        }

        FreeState();
        state_ = new State{1, new_array, size, size};
        state_->values[at] = value;
    }

private:
    State* state_;

    void FreeState() {
        auto old_state = state_;
        state_ = nullptr;

        if (old_state->count > 1) {
            --old_state->count;
            return;
        }

        delete[] old_state->values;
        delete old_state;
    }
};
