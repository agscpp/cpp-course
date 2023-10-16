#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>

template <class Iterator>
class Sequence {
public:
    Sequence(Iterator begin, Iterator end) : begin_{begin}, end_{end} {
    }

    Iterator begin() const {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }

private:
    Iterator begin_, end_;
};

template <class Iterator>
class RepeatIterator {
public:
    RepeatIterator(Iterator begin, Iterator end, size_t count = 0)
        : begin_{begin}, it_{begin}, end_{end}, count_{count} {
    }

    RepeatIterator& operator++() {
        if (!(++it_ != end_)) {
            ++count_;
            it_ = begin_;
        }
        return *this;
    }

    auto operator*() const {
        return *it_;
    }

    bool operator==(const RepeatIterator& rhs) const = default;

private:
    Iterator begin_, it_, end_;
    size_t count_;
};

class RangeIterator {
public:
    RangeIterator(int64_t size) : RangeIterator(0, size) {
    }

    RangeIterator(int64_t from, int64_t to, int64_t step = 1) : it_(from), to_(to), step_(step) {
    }

    RangeIterator& operator++() {
        it_ += step_;
        return *this;
    }

    auto operator*() const {
        return it_;
    }

    bool operator==(const RangeIterator& rhs) const {
        return this->it_ >= rhs.it_ && this->to_ == rhs.to_;
    }

    bool operator!=(const RangeIterator& rhs) const {
        return this->it_ < rhs.it_ || this->to_ != rhs.to_;
    }

private:
    int64_t it_;
    int64_t to_;
    int64_t step_;
};

template <class IteratorA, class IteratorB>
class ZipIterator {
public:
    ZipIterator(IteratorA begin_a, IteratorB begin_b) : begin_a_(begin_a), begin_b_(begin_b) {
    }

    ZipIterator& operator++() {
        ++begin_a_;
        ++begin_b_;
        return *this;
    }

    auto operator*() const {
        return std::pair(*begin_a_, *begin_b_);
    }

    bool operator==(const ZipIterator& rhs) const {
        return this->begin_a_ == rhs.begin_a_ || this->begin_b_ == rhs.begin_b_;
    }

    bool operator!=(const ZipIterator& rhs) const {
        return this->begin_a_ != rhs.begin_a_ && this->begin_b_ != rhs.begin_b_;
    }

private:
    IteratorA begin_a_;
    IteratorB begin_b_;
};

template <class Iterator>
class GroupIterator {
public:
    GroupIterator(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    GroupIterator& operator++() {
        auto value = *begin_;
        while (begin_ != end_) {
            if (*begin_ != value) {
                break;
            }
            ++begin_;
        }
        return *this;
    }

    auto operator*() const {
        auto it = begin_;
        auto value = *it;

        while (it != end_) {
            if (*it != value) {
                break;
            }
            ++it;
        }

        return Sequence(begin_, it);
    }

    bool operator==(const GroupIterator& rhs) const {
        return this->begin_ == rhs.begin_ && this->end_ == rhs.end_;
    }

    bool operator!=(const GroupIterator& rhs) const {
        return this->begin_ != rhs.begin_ || this->end_ != rhs.end_;
    }

private:
    Iterator begin_;
    Iterator end_;
};

auto Range(int64_t from, int64_t to, int64_t step = 1) {
    auto begin = RangeIterator{from, to, step};
    auto end = RangeIterator{to, to, step};
    return Sequence{begin, end};
}

auto Range(int64_t size) {
    return Range(0, size);
}

auto Zip(const auto& sequence_a, const auto& sequence_b) {
    auto begin = ZipIterator{sequence_a.begin(), sequence_b.begin()};
    auto end = ZipIterator{sequence_a.end(), sequence_b.end()};
    return Sequence{begin, end};
}

auto Group(const auto& sequence) {
    auto begin = GroupIterator{sequence.begin(), sequence.end()};
    auto end = GroupIterator{sequence.end(), sequence.end()};
    return Sequence{begin, end};
}

auto Repeat(const auto& sequence, size_t n) {
    auto begin = sequence.begin();
    auto end = sequence.end();
    if (n && (begin != end)) {
        return Sequence{RepeatIterator{begin, end}, RepeatIterator{begin, end, n}};
    } else {
        return Sequence{RepeatIterator{end, end}, RepeatIterator{end, end}};
    }
}
