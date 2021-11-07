#pragma once

#include <memory>
#include <vector>

#include <math/Vector.h>

#include <fractals/Fractal.h>

namespace CGCP::fractal {
    class JuliaParametrized : public Fractal {
    public:
        JuliaParametrized() = default;

        ~JuliaParametrized() override = default;

        // float raycast(math::Vector3 const &ro, math::Vector3 const &rd, math::Vector4 &rescol, float px) override;

        float raycast(
                math::Vector3 const &ro,
                math::Vector3 const &rd,
                math::Vector4 &rescol,
                float fov,
                math::Vector3 const &c) override;

        // math::Vector3 calcNormal(math::Vector3 &pos, float t, float px) override;

        math::Vector3 calcNormal(math::Vector3 &pos,
                                 float t,
                                 float fovfactor,
                                 const math::Vector3 &c) override;

    protected:
        virtual math::Vector2 isphere(math::Vector4 const &sph, math::Vector3 const &ro, math::Vector3 const &rd);

        float map(const math::Vector3 &p, const math::Vector3 &c, math::Vector4 &resColor);
    };
}// namespace CGCP::fractal
