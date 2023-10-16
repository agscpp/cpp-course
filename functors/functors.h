#pragma once

#include <algorithm>
#include <cstddef>

template <class Functor>
class ReverseUnaryFunctor {
private:
    const Functor& functor_;

public:
    ReverseUnaryFunctor() = delete;

    ReverseUnaryFunctor(const Functor& functor) : functor_(functor) {
    }

    template <class T>
    bool operator()(const T& a) const {
        return !functor_(a);
    }
};

template <class Functor>
class ReverseBinaryFunctor {
private:
    const Functor& functor_;

public:
    ReverseBinaryFunctor() = delete;

    ReverseBinaryFunctor(const Functor& functor) : functor_(functor) {
    }

    template <class T>
    bool operator()(const T& a, const T& b) const {
        return functor_(b, a);
    }
};

class CountComparator {
private:
    std::size_t* count_;

public:
    CountComparator() = delete;

    CountComparator(std::size_t* count) : count_(count) {
    }

    template <class T>
    bool operator()(const T& a, const T& b) {
        ++(*count_);
        return a < b;
    }
};

auto MakeReverseUnaryFunctor(auto functor) {
    return ReverseUnaryFunctor{functor};
}

auto MakeReverseBinaryFunctor(auto functor) {
    return ReverseBinaryFunctor{functor};
}

template <class Iterator>
size_t ComparisonsCount(Iterator first, Iterator last) {
    std::size_t count = 0;
    std::sort(first, last, CountComparator(&count));
    return count;
}
