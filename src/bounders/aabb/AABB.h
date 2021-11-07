#pragma once

#include <bounders/Bounder.h>

namespace CGCP::bounder {
    class AABB : public Bounder {
    public:
        AABB() = delete;

        AABB(math::Vector3 leftBottom, math::Vector3 topRight);

        math::Vector2 intersect(math::Vector3 const &ro,
                                math::Vector3 const &rd) override;

    private:
        math::Vector3 leftBottom_, topRight_;
    };
}// namespace CGCP::bounder
