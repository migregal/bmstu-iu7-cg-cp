#include <fractals/Fractal.h>

namespace CGCP::fractal {
    void Fractal::setBounder(std::shared_ptr<bounder::Bounder> bounder) {
        bounder_ = bounder;
    }

    void Fractal::setMaterial(const material::Material &material) {
        material_ = material;
    }

    math::Vector3 &Fractal::getColor() {
        return material_.getColor();
    }

    double Fractal::getRoughness() {
        return material_.getRoughness();
    }

    double Fractal::getSpecular() {
        return material_.getSpecular();
    }
}// namespace CGCP::fractal
