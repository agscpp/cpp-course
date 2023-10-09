#pragma once

#include <algorithm>
#include <vector>
#include <string>

class StaticMap {
public:
    explicit StaticMap(const std::vector<std::pair<std::string, std::string>>& items) {
        items_ = items;
        std::sort(items_.begin(), items_.end(), CompareByKey);
    }

    bool Find(const std::string& key, std::string* value) const {
        if (!items_.empty()) {
            int64_t left = 0;
            int64_t right = items_.size() - 1;

            while (left <= right) {
                int64_t middle = (right + left) / 2;
                const std::pair<std::string, std::string>& item = items_[middle];

                if (item.first < key) {
                    left = middle + 1;
                } else if (item.first > key) {
                    right = middle - 1;
                } else {
                    *value = item.second;
                    return true;
                }
            }
        }

        return false;
    }

private:
    std::vector<std::pair<std::string, std::string>> items_;

    static bool CompareByKey(const std::pair<std::string, std::string>& left,
                             const std::pair<std::string, std::string>& right) {
        return left.first < right.first;
    }
};
