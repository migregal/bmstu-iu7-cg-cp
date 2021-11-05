#pragma once

#include <memory>
#include <vector>

#include <math/Vector.h>

namespace CGCP::fractal {
    class Fractal {
    public:
        Fractal() = default;

        virtual float raycast(
                math::Vector3 const &ro,
                math::Vector3 const &rd,
                math::Vector4 &rescol,
                float fov,
                math::Vector3 const &c) = 0;

        virtual math::Vector3 calcNormal(math::Vector3 &pos,
                                         float t,
                                         float fovfactor,
                                         const math::Vector3 &c) = 0;

        virtual ~Fractal() = default;
    };
}// namespace CGCP::fractal
