#include <fractals/mandelbulb/MandelbulbParametrized.h>

#include <algorithm>
#include <cmath>

#include <math/Vector.h>

namespace CGCP::fractal {

    float MandelbulbParametrized::raycast(
            math::Vector3 const &ro,
            math::Vector3 const &rd,
            math::Vector4 &rescol,
            float fov,
            math::Vector3 const &c) {
        float res = -1.0;

        // bounding sphere
        auto dis = isphere(math::Vector4(0.0, 0.0, 0.0, 1.25), ro, rd);
        if (dis.y() < 0.0) return -1.0;
        dis.setX(std::max(dis.x(), 0.0f));

        // raymarch fractal distance field
        math::Vector4 trap;

        float fovfactor = 1.0 / sqrt(1.0 + fov * fov);
        float t = dis.x();
        for (int i = 0; i < 256; i++) {
            auto pos = ro + rd * t;
            float surface = std::clamp(0.001f * t * fovfactor, 0.0001f, 0.1f);

            float dt = map(pos, c, trap);
            if (t > dis.y() || dt < surface) break;
            t += std::min(dt, 0.05f);
        }

        if (t < dis.y()) {
            rescol = trap;
            res = t;
        }

        return res;
    }

    math::Vector3 MandelbulbParametrized::calcNormal(math::Vector3 &pos,
                                                     float t,
                                                     float fovfactor,
                                                     const math::Vector3 &c) {
        math::Vector4 tmp;
        float surface = std::clamp(0.0005f * t * fovfactor, 0.0f, 0.1f);
        auto eps = math::Vector2(surface, 0.0);
        auto e_xyy = math::Vector3(eps.x(), eps.y(), eps.y()),
             e_yyx = math::Vector3(eps.y(), eps.y(), eps.x()),
             e_yxy = math::Vector3(eps.y(), eps.x(), eps.y()),
             e_xxx = math::Vector3(eps.x(), eps.x(), eps.x());
        return math::Vector3(
                       map(pos + e_xyy, c, tmp) - map(pos - e_xyy, c, tmp),
                       map(pos + e_yxy, c, tmp) - map(pos - e_yxy, c, tmp),
                       map(pos + e_yyx, c, tmp) - map(pos - e_yyx, c, tmp))
                .normalized();
    }

    float MandelbulbParametrized::map(const math::Vector3 &p, const math::Vector3 &c, math::Vector4 &resColor) {
        math::Vector3 z = p;
        float m = math::Vector3::dotProduct(z, z);

        auto trap = math::Vector4(std::abs(z.x()), std::abs(z.y()), std::abs(z.z()), m);
        float dz = 1.0;

        for (int i = 0; i < 4; i++) {
            // dz = 8*z^7*dz
            dz = 8.0 * std::pow(m, 3.5) * dz;

            // z = z^8+z
            float r = z.length();
            float b = 8.0 * std::acos(std::clamp(z.y() / r, -1.0f, 1.0f));
            float a = 8.0 * std::atan2(z.x(), z.z());

            z = c + std::pow(r, 8.0) *
                            math::Vector3(std::sin(b) * std::sin(a),
                                          std::cos(b),
                                          std::sin(b) * std::cos(a));

            trap.setX(std::min(trap.x(), std::abs(z.x())));
            trap.setY(std::min(trap.y(), std::abs(z.y())));
            trap.setZ(std::min(trap.z(), std::abs(z.z())));
            trap.setZ(std::min(trap.w(), m));

            m = math::Vector3::dotProduct(z, z);
            if (m > 2.0)
                break;
        }

        // resColor = math::Vector4(m, trap.y(), trap.z(), trap.w());
        resColor = trap;

        // distance estimation (through the Hubbard-Douady potential)
        return 0.25 * log(m) * sqrt(m) / dz;
    }

    math::Vector2 MandelbulbParametrized::isphere(math::Vector4 const &sph, math::Vector3 const &ro, math::Vector3 const &rd) {
        auto oc = ro - math::Vector3(sph.x(), sph.y(), sph.z());

        float b = math::Vector3::dotProduct(oc, rd);
        float c = math::Vector3::dotProduct(oc, oc) - sph.w() * sph.w();

        float h = b * b - c;

        if (h < 0.0) return math::Vector2(-1.0, -1.0);

        h = sqrt(h);

        return math::Vector2(-h - b, h - b);
    }
}// namespace CGCP::fractal
