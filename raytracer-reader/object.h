#pragma once

#include <triangle.h>
#include <material.h>
#include <sphere.h>
#include <vector.h>

struct Object {
    const Material* material = nullptr;
    Triangle polygon;
    Triangle texture;
    Triangle normal;

    const Vector* GetNormal(size_t index) const {
        return &normal[index];
    }

    Object(const Triangle& polygon, const Triangle& texture, const Triangle& normal)
        : polygon(polygon), texture(texture), normal(normal) {
    }
};

struct SphereObject {
    const Material* material = nullptr;
    Sphere sphere;

    SphereObject(const Sphere& sphere) : sphere(sphere) {
    }
};
