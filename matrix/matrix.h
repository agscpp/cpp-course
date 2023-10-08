#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

class Matrix {
public:
    Matrix(std::size_t size) : Matrix(size, size) {
    }

    Matrix(std::size_t rows, std::size_t columns) : rows_(rows), columns_(columns) {
        matrix_.resize(rows_, std::vector<double>(columns_, 0));
    }

    Matrix(std::vector<std::vector<double>> matrix) : matrix_(matrix) {
        rows_ = matrix_.size();
        columns_ = (!matrix_.empty()) ? matrix_.front().size() : 0;
    }

    std::size_t Rows() const {
        return rows_;
    }

    std::size_t Columns() const {
        return columns_;
    }

    double& operator()(const std::size_t row, const std::size_t column) {
        assert(row < rows_ && column < columns_);
        return matrix_[row][column];
    }

    const double& operator()(const std::size_t row, const std::size_t column) const {
        assert(row < rows_ && column < columns_);
        return matrix_[row][column];
    }

    Matrix& operator*=(const Matrix& rhs) {
        assert(this->rows_ == rhs.columns_ && this->columns_ == rhs.rows_);
        return *this = *this * rhs;
    }

    Matrix& operator+=(const Matrix& rhs) {
        assert(this->rows_ == rhs.rows_ && this->columns_ == rhs.columns_);

        for (std::size_t i = 0; i < this->rows_; ++i) {
            for (std::size_t j = 0; j < this->columns_; ++j) {
                this->matrix_[i][j] += rhs.matrix_[i][j];
            }
        }

        return *this;
    }

    Matrix& operator-=(const Matrix& rhs) {
        assert(this->rows_ == rhs.rows_ && this->columns_ == rhs.columns_);

        for (std::size_t i = 0; i < this->rows_; ++i) {
            for (std::size_t j = 0; j < this->columns_; ++j) {
                this->matrix_[i][j] -= rhs.matrix_[i][j];
            }
        }

        return *this;
    }

    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);

private:
    std::vector<std::vector<double>> matrix_;
    std::size_t rows_;
    std::size_t columns_;
};

Matrix Transpose(const Matrix& matrix) {
    Matrix answer(matrix.Columns(), matrix.Rows());

    for (std::size_t i = 0; i < answer.Rows(); ++i) {
        for (std::size_t j = 0; j < answer.Columns(); ++j) {
            answer(i, j) = matrix(j, i);
        }
    }

    return answer;
}

Matrix Identity(size_t n) {
    Matrix matrix(n);
    for (std::size_t i = 0; i < n; ++i) {
        matrix(i, i) = 1;
    }
    return matrix;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    assert(lhs.rows_ == rhs.columns_ && lhs.columns_ == rhs.rows_);

    Matrix answer(lhs.rows_, rhs.columns_);
    for (std::size_t i = 0; i < answer.rows_; ++i) {
        for (std::size_t j = 0; j < answer.columns_; ++j) {
            for (std::size_t k = 0; k < rhs.rows_; ++k) {
                answer(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }

    return answer;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    assert(lhs.rows_ == rhs.rows_ && lhs.columns_ == rhs.columns_);
    Matrix answer = lhs;
    answer += rhs;
    return answer;
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    assert(lhs.rows_ == rhs.rows_ && lhs.columns_ == rhs.columns_);
    Matrix answer = lhs;
    answer -= rhs;
    return answer;
}
