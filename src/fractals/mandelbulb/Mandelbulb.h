#pragma once

#include <memory>
#include <vector>

#include <math/Vector.h>

#include <fractals/julia/Julia.h>

namespace CGCP::fractal {
    class Mandelbulb : public JuliaParametrized {
    public:
        Mandelbulb() = default;

        ~Mandelbulb() override = default;

        float raycast(
                math::Vector3 const &ro,
                math::Vector3 const &rd,
                math::Vector4 &rescol,
                float fov,
                math::Vector3 const &c) override;

    protected:
        float map(math::Vector3 p, math::Vector4 &resColor);
    };
}// namespace CGCP::fractal
