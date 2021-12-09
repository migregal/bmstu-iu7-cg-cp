#pragma once

#include <math/Vector.h>

namespace CGCP::material {
    class Material {
    public:
        Material() = default;

        Material(double roughness, double specular, math::Vector3 color);

        math::Vector3 &getColor();

        double getRoughness();

        double getSpecular();

    private:
        double roughness_;
        double specular_;
        math::Vector3 color_;
    };
}// namespace CGCP::material
