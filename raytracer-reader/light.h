#pragma once

#include <vector.h>

struct Light {
    Vector position;
    Vector intensity;

    Light(const Vector& position, const Vector& intensity)
        : position(position), intensity(intensity) {
    }
};
