#pragma once

#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace tag {

inline constexpr struct Exception {
} kEx;

}  // namespace tag

template <class T>
class Try {
public:
    Try() = default;

    Try(const T& object) : object_(std::make_unique<T>(object)) {
    }

    Try(T&& object) : object_(std::make_unique<T>(std::forward<T>(object))) {
    }

    template <typename P>
    Try(tag::Exception, P&& ex) : ex_(std::make_exception_ptr(std::forward<P>(ex))) {
    }

    Try(tag::Exception, std::exception_ptr ex) : ex_(ex) {
    }

    Try(const Try<T>&) = delete;
    Try(Try<T>&&) = delete;

    Try& operator=(const Try<T>&) = delete;
    Try& operator=(Try<T>&&) = delete;

    bool IsFailed() const {
        return ex_ != nullptr;
    }

    const T& Value() const {
        if (ex_) {
            std::rethrow_exception(ex_);
        }
        if (object_) {
            return *object_;
        }
        throw std::runtime_error("Object is empty");
    }
    void Throw() const {
        if (ex_) {
            std::rethrow_exception(ex_);
        }
        throw std::runtime_error("No exception");
    }

private:
    std::unique_ptr<T> object_ = nullptr;
    std::exception_ptr ex_ = nullptr;
};

template <>
class Try<void> {
public:
    Try() = default;

    template <typename P>
    Try(tag::Exception, P&& ex) : ex_(std::make_exception_ptr(std::forward<P>(ex))) {
    }

    Try(tag::Exception, std::exception_ptr ex) : ex_(ex) {
    }

    Try(const Try<void>&) = delete;
    Try(Try<void>&&) = delete;

    Try& operator=(const Try<void>&) = delete;
    Try& operator=(Try<void>&&) = delete;

    bool IsFailed() const {
        return ex_ != nullptr;
    }

    void Throw() const {
        if (ex_) {
            std::rethrow_exception(ex_);
        }
        throw std::runtime_error("No exception");
    }

private:
    std::exception_ptr ex_ = nullptr;
};

template <class Function, class... Args>
auto TryRun(Function func, Args&&... args) {
    using ReturnType = decltype(func(std::forward<Args>(args)...));
    try {
        if constexpr (std::is_void_v<ReturnType>) {
            func(std::forward<Args>(args)...);
            return Try<ReturnType>();
        } else {
            return Try<ReturnType>(func(std::forward<Args>(args)...));
        }
    } catch (const std::exception&) {
        return Try<ReturnType>(tag::kEx, std::current_exception());
    } catch (const char* str) {
        return Try<ReturnType>(tag::kEx, std::runtime_error(str));
    } catch (int value) {
        return Try<ReturnType>(tag::kEx, std::system_error(value, std::generic_category()));
    } catch (...) {
        return Try<ReturnType>(tag::kEx, std::runtime_error("Unknown exception"));
    }
}
