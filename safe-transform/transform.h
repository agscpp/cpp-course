#pragma once

#include <exception>
#include <optional>
#include <vector>

class TransformException : public std::exception {
public:
    TransformException(std::exception_ptr original_exception) : original_(original_exception) {
    }

    std::exception_ptr GetOriginalException() {
        return original_;
    }

private:
    std::exception_ptr original_;
};

namespace {
template <typename ValueIterator, typename LogIterator>
bool SaveValue(ValueIterator value, LogIterator log) {
    try {
        *log = *value;
        return true;
    } catch (...) {
        return false;
    }
}

template <typename ValueIterator, typename LogIterator, typename P, typename F>
void Run(ValueIterator value, LogIterator log, const P& p, const F& f, bool* is_log_correct) {
    try {
        if (p(*value)) {
            if (*is_log_correct) {
                *is_log_correct = SaveValue(value, log);
            }
            f(*value);
        }
    } catch (...) {
        if (*is_log_correct) {
            throw TransformException(std::current_exception());
        }
        throw;
    }
}
}  // namespace

template <typename Iterator, typename P, typename F>
void TransformIf(Iterator begin, Iterator end, P p, const F f) {
    std::vector<std::optional<typename Iterator::value_type>> log;
    auto is_log_correct = true;
    for (auto value_it = begin; value_it != end; ++value_it) {
        try {
            if (is_log_correct) {
                log.push_back(std::nullopt);
            }
            Run(value_it, log.end() - 1, p, f, &is_log_correct);
        } catch (TransformException& ex) {
            for (size_t index = 0; index < log.size(); ++index, ++begin) {
                if (auto& value = log[index]) {
                    *begin = std::move(*value);
                }
            }
            std::rethrow_exception(ex.GetOriginalException());
        }
    }
}