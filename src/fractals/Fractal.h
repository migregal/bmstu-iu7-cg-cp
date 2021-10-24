#pragma once

#include <memory>
#include <vector>

#include <math/Vector.h>

namespace CGCP::fractal {
    class Fractal {
    public:
        Fractal() = default;

        virtual float raycast(math::Vector3 const &ro, math::Vector3 const &rd, QVector4D &rescol, float px) = 0;

        virtual math::Vector3 calcNormal(math::Vector3 &pos, float t, float px) = 0;

        virtual float softshadow(math::Vector3 const &ro, math::Vector3 const &rd, float k) = 0;

        virtual ~Fractal() = default;
    };
}// namespace CGCP::fractal