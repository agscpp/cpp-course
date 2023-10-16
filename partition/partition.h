#pragma once

#include <utility>

template <class Iterator, class Predicate>
Iterator Partition(Iterator first, Iterator last, Predicate pred) {
    auto answer = first;

    while (first != last) {
        if (pred(*first)) {
            std::swap(*first, *answer);
            ++answer;
        }
        ++first;
    }

    return answer;
}
