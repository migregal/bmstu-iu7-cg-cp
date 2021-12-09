#include <fractals/mandelbulb/Mandelbulb.h>

#include <algorithm>
#include <cmath>

namespace CGCP::fractal {

    float Mandelbulb::raycast(
            math::Vector3 const &ro,
            math::Vector3 const &rd,
            math::Vector4 &rescol,
            float fov,
            math::Vector3 const &c) {
        float res = -1.0;

        auto dis = bounder_->intersect(ro, rd);

        if (dis.y() < 0.0) return -1.0;
        dis.setX(std::max(dis.x(), 0.0f));
        dis.setY(std::min(dis.y(), 10.0f));

        // raymarch fractal distance field
        math::Vector4 trap;

        float fovfactor = 1.0 / sqrt(1.0 + fov * fov);
        float t = dis.x();
        for (int i = 0; i < 256; i++) {
            auto pos = ro + rd * t;
            float surface = std::clamp(0.001f * t * fovfactor, 0.0f, 1e-8f);
            float h = map(pos, trap);
            if (t > dis.y() || h < surface) break;
            t += h;
        }

        if (t < dis.y()) {
            rescol = trap;
            res = t;
        }

        return res;
    }

    float Mandelbulb::map(math::Vector3 p, math::Vector4 &resColor) {
        math::Vector3 w = p;
        float m = math::Vector3::dotProduct(w, w);

        auto trap = math::Vector4(std::abs(w.x()), std::abs(w.y()), std::abs(w.z()), m);
        float dz = 1.0;

        for (int i = 0; i < 4; i++) {
#if 1
            // polynomial version (no trigonometrics, but MUCH slower)
            float m2 = m * m;
            float m4 = m2 * m2;
            dz = 8.0 * std::sqrt(m4 * m2 * m) * dz + 1.0;

            float x = w.x();
            float x2 = x * x;
            float x4 = x2 * x2;
            float y = w.y();
            float y2 = y * y;
            float y4 = y2 * y2;
            float z = w.z();
            float z2 = z * z;
            float z4 = z2 * z2;

            float k3 = x2 + z2;
            float k2 = 1 / std::sqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
            float k1 = x4 + y4 + z4 - 6.0 * y2 * z2 - 6.0 * x2 * y2 + 2.0 * z2 * x2;
            float k4 = x2 - y2 + z2;

            w.setX(p.x() + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2);
            w.setY(p.y() + -16.0 * y2 * k3 * k4 * k4 + k1 * k1);
            w.setZ(p.z() + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4 + z4 * z4) * k1 * k2);
#else
            // dz = 8*z^7*dz
            dz = 8.0 * std::pow(m, 3.5) * dz + 1.0;

            // z = z^8+z
            float r = w.length();
            float b = 8.0 * std::acos(w.y() / r);
            float a = 8.0 * std::atan2(w.x(), w.z());
            w = p + std::pow(r, 8.0) *
                            math::Vector3(std::sin(b) * std::sin(a),
                                          std::cos(b),
                                          std::sin(b) * std::cos(a));
#endif
            trap.setX(std::min(trap.x(), std::abs(w.x())));
            trap.setY(std::min(trap.y(), std::abs(w.y())));
            trap.setZ(std::min(trap.z(), std::abs(w.z())));
            trap.setZ(std::min(trap.w(), m));

            m = math::Vector3::dotProduct(w, w);
            if (m > 256.0)
                break;
        }

        resColor = math::Vector4(m, trap.y(), trap.z(), trap.w());

        // distance estimation (through the Hubbard-Douady potential)
        return 0.25 * log(m) * sqrt(m) / dz;
    }
}// namespace CGCP::fractal
