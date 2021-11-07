#pragma once

#include <bounders/Bounder.h>

namespace CGCP::bounder {
    class Sphere : public Bounder {
        Sphere(math::Vector4 const &sphere);

        math::Vector2 intersect(math::Vector3 const &ro,
                                math::Vector3 const &rd) override;

    private:
        math::Vector4 sphere_;
    };
}// namespace CGCP::bounder
