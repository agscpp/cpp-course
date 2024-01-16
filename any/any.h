#pragma once

#include <new>
#include <concepts>
#include <typeinfo>
#include <utility>

template <class T>
concept NotAny = !std::same_as<std::remove_cvref_t<T>, class Any>;

class Any {
public:
    Any() {
    }

    template <NotAny T>
    Any(T&& value) : holder_(new ValueHolder(std::forward<T>(value))) {
    }

    Any(const Any& other) {
        if (other.holder_) {
            holder_ = other.holder_->Clone();
        }
    }

    Any(Any&& other) : holder_(other.holder_) {
        other.holder_ = nullptr;
    }

    Any& operator=(const Any& other) {
        if (this != &other) {
            auto new_any = other;
            Swap(new_any);
        }
        return *this;
    }

    Any& operator=(Any&& other) {
        if (this != &other) {
            Swap(other);
        }
        return *this;
    }

    ~Any() {
        Clear();
    }

    bool Empty() const {
        return holder_ == nullptr;
    }

    void Clear() {
        delete holder_;
        holder_ = nullptr;
    }
    void Swap(Any& other) {
        std::swap(holder_, other.holder_);
    }

    template <class T>
    const T& GetValue() const {
        auto* ptr = dynamic_cast<ValueHolder<T>*>(holder_);
        if (ptr) {
            return *(*ptr);
        }
        throw std::bad_cast();
    }

private:
    class ValueHolderBase {
    public:
        virtual ~ValueHolderBase() = default;
        virtual ValueHolderBase* Clone() = 0;
    };

    template <typename T>
    class ValueHolder : public ValueHolderBase {
    public:
        ValueHolder(T value) : value_(std::move(value)) {
        }

        T& operator*() {
            return value_;
        }

        const T& operator*() const {
            return value_;
        }

        ValueHolder<T>* Clone() override {
            return new ValueHolder<T>(value_);
        }

    private:
        T value_;
    };

    ValueHolderBase* holder_ = nullptr;
};
