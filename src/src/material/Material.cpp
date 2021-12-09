#include <material/Material.h>

namespace CGCP::material {
    Material::Material(double roughness, double specular, math::Vector3 color)
        : roughness_(roughness), specular_(specular), color_(color) {}

    math::Vector3 &Material::getColor() {
        return color_;
    }

    double Material::getRoughness() {
        return roughness_;
    }

    double Material::getSpecular() {
        return specular_;
    }
}// namespace CGCP::material
