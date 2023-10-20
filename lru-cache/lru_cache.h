#pragma once

#include <cstddef>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>

class LruCache {
public:
    explicit LruCache(size_t max_size) : max_size_(max_size) {
    }
    LruCache(const LruCache&) = delete;
    LruCache& operator=(const LruCache&) = delete;

    void Set(const std::string& key, const std::string& value) {
        if (iters_.contains(key)) {
            data_.erase(iters_.at(key));
        }

        if (data_.size() == max_size_) {
            auto old_data = data_.back();
            iters_.erase(old_data.first);
            data_.pop_back();
        }

        data_.push_front({key, value});
        iters_[key] = data_.begin();
    }

    bool Get(const std::string& key, std::string* value) {
        if (!iters_.contains(key)) {
            return false;
        }

        auto iter = iters_.at(key);

        data_.push_front(*iter);
        data_.erase(iter);
        iters_[key] = data_.begin();

        *value = data_.front().second;
        return true;
    }

private:
    typedef std::list<std::pair<std::string, std::string>> Data;

    Data data_;
    std::unordered_map<std::string, Data::iterator> iters_;
    std::size_t max_size_;
};
