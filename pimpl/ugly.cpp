#include <algorithm>
#include <cstddef>
#include <memory>
#include <new>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ugly.h"
#include "bad.h"

class Spline::SplineImpl {
public:
    SplineImpl(std::vector<double>&& x, std::vector<double>&& y, double a, double b)
        : n_(x.size()), x_(std::move(x)), y_(std::move(y)) {
        for (size_t i = 1; i < n_; ++i) {
            if (x_[i - 1] >= x_[i]) {
                throw std::invalid_argument("xa[i] < xa[i+1] not equal");
            }
        }

        y2_ = new double[n_];
        int return_code = mySplineSnd(x_.data(), y_.data(), n_, a, b, y2_);
        ThrowExceptionIfNeeded(return_code, true);
    }

    ~SplineImpl() {
        delete[] y2_;
    }

    double Interpolate(double x) const {
        double y = 0;
        int return_code = mySplintCube(x_.data(), y_.data(), y2_, n_, x, &y);
        ThrowExceptionIfNeeded(return_code);
        return y;
    }

private:
    size_t n_;
    std::vector<double> x_;
    std::vector<double> y_;
    double* y2_;

    void ThrowExceptionIfNeeded(int code, bool flag = false) const {
        switch (code) {
            case -1:
                if (flag) {
                    throw std::bad_alloc();
                }
                throw std::runtime_error("Array length is 0");
            case -2:
                throw std::runtime_error("x is not monitonic increasing");
        }
    }
};

Spline::Spline(std::vector<double> x, std::vector<double> y, double a, double b)
    : impl_(std::make_unique<SplineImpl>(std::move(x), std::move(y), a, b)) {
}

Spline::~Spline() {
}

double Spline::Interpolate(double x) const {
    return impl_->Interpolate(x);
}