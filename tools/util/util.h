#pragma once

#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <cerrno>
#include <system_error>
#include <chrono>

#ifdef __linux__
#include <sys/time.h>
#include <sys/resource.h>
#endif

class RandomGenerator {
public:
    explicit RandomGenerator(uint32_t seed = 738'547'485u) : gen_{seed} {
    }

    template <class T>
    std::vector<T> GenIntegralVector(size_t count, T from, T to) {
        std::uniform_int_distribution dist{from, to};
        std::vector<T> result(count);
        for (auto& cur : result) {
            cur = dist(gen_);
        }
        return result;
    }

    std::string GenString(size_t count, char from = 'a', char to = 'z') {
        std::uniform_int_distribution<int> dist{from, to};
        std::string result(count, from);
        for (auto& x : result) {
            x = dist(gen_);
        }
        return result;
    }

    std::vector<double> GenRealVector(size_t count, double from, double to) {
        std::uniform_real_distribution dist{from, to};
        std::vector<double> result(count);
        for (auto& cur : result) {
            cur = dist(gen_);
        }
        return result;
    }

    std::vector<int> GenPermutation(size_t count) {
        std::vector<int> result(count);
        std::iota(result.begin(), result.end(), 0);
        std::ranges::shuffle(result, gen_);
        return result;
    }

    template <class T>
    T GenInt(T from, T to) {
        std::uniform_int_distribution dist{from, to};
        return dist(gen_);
    }

    template <class T>
    T GenInt() {
        return GenInt(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    template <class Iterator>
    void Shuffle(Iterator first, Iterator last) {
        std::shuffle(first, last, gen_);
    }

private:
    std::mt19937 gen_;
};

inline std::filesystem::path GetFileDir(std::string file) {
    std::filesystem::path path{std::move(file)};
    if (path.is_absolute() && std::filesystem::is_regular_file(path)) {
        return path.parent_path();
    } else {
        throw std::runtime_error{"Bad file name"};
    }
}

class Timer {
    using Clock = std::chrono::steady_clock;
    struct Times {
        Clock::duration wall_time;
        std::chrono::microseconds cpu_time; 
    };

public:
    Times GetTimes() const {
        return {Clock::now() - wall_start_, GetCPUTime() - cpu_start_};
    }

private:

    static std::chrono::microseconds GetCPUTime() {
#ifdef __linux__
        if (rusage usage; ::getrusage(RUSAGE_SELF, &usage)) {
            throw std::system_error{errno, std::generic_category()};
        } else {
            auto time = usage.ru_utime;
            return std::chrono::microseconds{1'000'000ll * time.tv_sec + time.tv_usec};
        }
#else
        auto time = Clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::microseconds>(time);
#endif
    }

    const std::chrono::time_point<Clock> wall_start_ = Clock::now();
    const std::chrono::microseconds cpu_start_ = GetCPUTime(); 
};

#ifdef __linux__
inline int64_t GetMemoryUsage() {
    if (rusage usage; ::getrusage(RUSAGE_SELF, &usage)) {
        throw std::system_error{errno, std::generic_category()};
    } else {
        return usage.ru_maxrss;
    }
}
#endif
