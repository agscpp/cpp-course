#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <ranges>

struct Student {
    std::string name, surname;
    int day, month, year;

    bool operator==(const Student& rhs) const {
        return this->name == rhs.name && this->surname == rhs.surname && this->year == rhs.year &&
               this->month == rhs.month && this->day == rhs.day;
    }
};

enum class SortType { kByName, kByDate };

namespace {
bool CompareByName(const Student& left, const Student& right) {
    if (left.surname == right.surname) {
        if (left.name == right.name) {
            if (left.year == right.year) {
                if (left.month == right.month) {
                    return left.day < right.day;
                }
                return left.month < right.month;
            }
            return left.year < right.year;
        }
        return left.name < right.name;
    }
    return left.surname < right.surname;
}

bool CompareByDate(const Student& left, Student& right) {
    if (left.year == right.year) {
        if (left.month == right.month) {
            if (left.day == right.day) {
                if (left.surname == right.surname) {
                    return left.name < right.name;
                }
                return left.surname < right.surname;
            }
            return left.day < right.day;
        }
        return left.month < right.month;
    }
    return left.year < right.year;
}
}  // namespace

void SortStudents(std::vector<Student>* students, SortType sort_type) {
    switch (sort_type) {
        case SortType::kByName:
            std::ranges::sort((*students).begin(), (*students).end(), CompareByName);
            break;
        case SortType::kByDate:
            std::ranges::sort((*students).begin(), (*students).end(), CompareByDate);
            break;
    }
}
