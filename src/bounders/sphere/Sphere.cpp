#include <bounders/sphere/Sphere.h>

#include <cmath>

namespace CGCP::bounder {
    Sphere::Sphere(math::Vector4 const &sphere)
        : sphere_(sphere) {}

    math::Vector2 Sphere::intersect(math::Vector3 const &ro, math::Vector3 const &rd) {
        auto oc = ro - math::Vector3(sphere_.x(), sphere_.y(), sphere_.z());

        float b = math::Vector3::dotProduct(oc, rd);
        float c = math::Vector3::dotProduct(oc, oc) - sphere_.w() * sphere_.w();

        float h = b * b - c;

        if (h < 0.0) return math::Vector2(-1.0, -1.0);

        h = std::sqrt(h);

        return math::Vector2(-h - b, h - b);
    }
}// namespace CGCP::bounder
