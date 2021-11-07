#include <bounders/aabb/AABB.h>

namespace CGCP::bounder {
    AABB::AABB(math::Vector3 leftBottom, math::Vector3 topRight)
        : leftBottom_(leftBottom), topRight_(topRight) {}

    math::Vector2 AABB::intersect(math::Vector3 const &ro,
                                  math::Vector3 const &rd) {
        auto dirfrac = math::Vector3(1.0f / rd.x(),
                                     1.0f / rd.y(),
                                     1.0f / rd.z());
        float t1 = (leftBottom_.x() - ro.x()) * dirfrac.x();
        float t2 = (topRight_.x() - ro.x()) * dirfrac.x();
        float t3 = (leftBottom_.y() - ro.y()) * dirfrac.y();
        float t4 = (topRight_.y() - ro.y()) * dirfrac.y();
        float t5 = (leftBottom_.z() - ro.z()) * dirfrac.z();
        float t6 = (topRight_.z() - ro.z()) * dirfrac.z();

        float tmin = std::max(
                std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(
                std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
        if (tmax < 0) return {-1, -1};

        // if tmin > tmax, ray doesn't intersect AABB
        if (tmin > tmax) return {-1, -1};

        return {tmin, tmax};
    }
}// namespace CGCP::bounder
