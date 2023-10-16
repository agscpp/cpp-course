#pragma once

template <class Iterator>
Iterator LocalMax(Iterator first, Iterator last) {
    if (first == last) {
        return last;
    }

    auto answer = first;
    auto first_min = first;

    while (++first != last) {
        if (answer == last) {
            if (*first_min < *first) {
                answer = first;
            } else {
                first_min = first;
            }
        } else if (*first < *answer) {
            return answer;
        } else if (*answer < *first) {
            first_min = answer;
            answer = first;
        } else {
            first_min = first;
            answer = last;
        }
    }

    return answer;
}
