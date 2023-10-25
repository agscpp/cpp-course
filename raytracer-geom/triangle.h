#pragma once

#include <vector.h>
#include <cmath>

class Triangle {
public:
    Triangle() = delete;

    Triangle(const Vector& a, const Vector& b, const Vector& c) : data_({a, b, c}) {
    }

    Vector& operator[](size_t ind) {
        return data_[ind];
    }

    const Vector& operator[](size_t ind) const {
        return data_[ind];
    }

    double Area() const {
        auto product = CrossProduct(data_[0] - data_[1], data_[0] - data_[2]);
        return product.GetLength() * 0.5;
    }

private:
    std::array<Vector, 3> data_;
};
