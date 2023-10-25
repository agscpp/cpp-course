#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <intersection.h>
#include <optional>
#include <math.h>
#include <ray.h>
#include <sphere.h>
#include <triangle.h>
#include <vector.h>

std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) {
    auto l = sphere.GetCenter() - ray.GetOrigin();
    auto t = DotProduct(l, ray.GetDirection());
    auto length = Length(l);

    if (t < 0 && length > sphere.GetRadius()) {
        return std::nullopt;
    }

    if (length * length < t * t) {
        return std::nullopt;
    }

    auto d_sq = DotProduct(l, l) - t * t;
    auto r_sq = sphere.GetRadius() * sphere.GetRadius();

    if (d_sq > r_sq) {
        return std::nullopt;
    }

    Vector position;
    auto t1 = std::sqrt(r_sq - d_sq);

    if (Length(ray.GetOrigin(), sphere.GetCenter()) < sphere.GetRadius()) {
        position = ray.GetOrigin() + (t + t1) * ray.GetDirection();
    } else {
        position = ray.GetOrigin() + (t - t1) * ray.GetDirection();
    }

    auto normal = position - sphere.GetCenter();
    if (Length(position + normal, ray.GetOrigin()) > Length(position - normal, ray.GetOrigin())) {
        normal *= -1;
    }

    normal.Normalize();
    return Intersection(position, normal, Length(ray.GetOrigin(), position));
}

std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) {
    auto edge1 = triangle[1] - triangle[0];
    auto edge2 = triangle[2] - triangle[0];

    auto p = CrossProduct(ray.GetDirection(), edge2);
    auto det = DotProduct(edge1, p);

    if (det < 1e-8 && det > -1e-8) {
        return std::nullopt;
    }

    auto inv_det = 1 / det;
    auto t = ray.GetOrigin() - triangle[0];
    auto u = DotProduct(t, p) * inv_det;

    if (u < 0 || u > 1) {
        return std::nullopt;
    }

    auto q = CrossProduct(t, edge1);
    auto v = DotProduct(ray.GetDirection(), q) * inv_det;

    if (v < 0 || u + v > 1) {
        return std::nullopt;
    }

    auto k = DotProduct(edge2, q) * inv_det;
    if (k < 0) {
        return std::nullopt;
    }

    auto position = ray.GetOrigin() + k * ray.GetDirection();
    auto normal = CrossProduct(edge1, edge2);

    normal.Normalize();
    if (Length(position + normal, ray.GetOrigin()) > Length(position - normal, ray.GetOrigin())) {
        normal *= -1;
    }

    return Intersection(position, normal, Length(ray.GetOrigin(), position));
}

Vector Reflect(const Vector& ray, const Vector& normal) {
    return ray - 2.0 * DotProduct(ray, normal) * normal;
}

std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) {
    auto cos_incidence = DotProduct(ray, normal);
    if (cos_incidence > 1 || cos_incidence < -1) {
        return std::nullopt;
    }

    if (cos_incidence < 0) {
        cos_incidence *= -1;
    }

    auto cos_refraction = 1 - eta * eta * (1 - cos_incidence * cos_incidence);
    if (cos_refraction < 0) {
        return std::nullopt;
    }

    return eta * ray + (eta * cos_incidence - std::sqrt(cos_refraction)) * normal;
}

Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) {
    double sum_areas = 0;
    std::array<double, 3> areas;

    for (size_t i = 0; i < areas.size(); ++i) {
        Triangle sub_triangle = triangle;
        sub_triangle[i] = point;
        areas[i] = sub_triangle.Area();
        sum_areas += areas[i];
    }

    Vector answer;
    for (size_t i = 0; i < areas.size(); ++i) {
        answer[i] = areas[i] / sum_areas;
    }

    return answer;
}
