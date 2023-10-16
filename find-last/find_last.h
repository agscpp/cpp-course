#pragma once

template <class Iterator, class T>
Iterator FindLast(Iterator first, Iterator last, const T& value) {
    auto answer = last;
    while (first != answer) {
        if (value == *(--answer)) {
            return answer;
        }
    }
    return last;
}
