#pragma once

#include <memory>
#include <vector>

#include <math/Vector.h>

#include <fractals/Fractal.h>

namespace CGCP::fractal {
    class Mandelbulb : public Fractal {
    public:
        Mandelbulb() = default;

        float raycast(math::Vector3 const &ro, math::Vector3 const &rd, math::Vector4 &rescol, float px) override;

        ~Mandelbulb() override = default;

        math::Vector3 calcNormal(math::Vector3 &pos, float t, float px) override;

        float softshadow(math::Vector3 const &ro, math::Vector3 const &rd, float k) override;

    private:
        math::Vector2 isphere(math::Vector4 const &sph, math::Vector3 const &ro, math::Vector3 const &rd);

        float map(math::Vector3 p, math::Vector4 &resColor);
    };
}// namespace CGCP::fractal