#pragma once

#include <array>
#include <cmath>
#include <cstddef>

class Vector {
public:
    Vector() : Vector(0, 0, 0) {
    }

    Vector(double x, double y, double z) : data_({x, y, z}) {
    }

    double& operator[](size_t ind) {
        return data_[ind];
    }

    double operator[](size_t ind) const {
        return data_[ind];
    }

    Vector& operator*=(double scalar) {
        for (auto& value : this->data_) {
            value *= scalar;
        }
        return *this;
    }

    Vector& operator/=(double scalar) {
        for (auto& value : this->data_) {
            value /= scalar;
        }
        return *this;
    }

    Vector& operator+=(const Vector& other) {
        for (size_t i = 0; i < 3; ++i) {
            (*this)[i] += other[i];
        }
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        for (size_t i = 0; i < 3; ++i) {
            (*this)[i] -= other[i];
        }
        return *this;
    }

    friend Vector operator*(double scalar, const Vector& vector);
    friend Vector operator*(const Vector& vector, double scalar);
    friend Vector operator/(const Vector& vector, double scalar);
    friend Vector operator+(const Vector& lhs, const Vector& rhs);
    friend Vector operator-(const Vector& lhs, const Vector& rhs);

    double GetLength() const {
        const auto& vector = (*this);
        return std::sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
    }

    void Normalize() {
        *this /= GetLength();
    }

private:
    std::array<double, 3> data_;
};

Vector operator*(const Vector& vector, double scalar) {
    auto new_vector = vector;
    return new_vector *= scalar;
}

Vector operator*(double scalar, const Vector& vector) {
    auto new_vector = vector;
    return new_vector *= scalar;
}

Vector operator/(const Vector& vector, double scalar) {
    auto new_vector = vector;
    return new_vector /= scalar;
}

Vector operator+(const Vector& lhs, const Vector& rhs) {
    auto new_vector = lhs;
    return new_vector += rhs;
}

Vector operator-(const Vector& lhs, const Vector& rhs) {
    auto new_vector = lhs;
    return new_vector -= rhs;
}

double DotProduct(const Vector& a, const Vector& b) {
    double product = 0;
    for (size_t i = 0; i < 3; ++i) {
        product += a[i] * b[i];
    }
    return product;
}

Vector CrossProduct(const Vector& a, const Vector& b) {
    Vector product;
    product[0] = (a[1] * b[2]) - (a[2] * b[1]);
    product[1] = -((a[0] * b[2]) - (a[2] * b[0]));
    product[2] = (a[0] * b[1]) - (a[1] * b[0]);
    return product;
}

double Length(const Vector& vector) {
    return vector.GetLength();
}

double Length(const Vector& vector1, const Vector& vector2) {
    auto new_vector = vector1 - vector2;
    return new_vector.GetLength();
}
