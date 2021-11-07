#pragma once

#include <math/Vector.h>

namespace CGCP::bounder {
    class Bounder {
    public:
        virtual math::Vector2 intersect(math::Vector3 const &ro,
                                        math::Vector3 const &rd) = 0;
    };
}// namespace CGCP::bounder
