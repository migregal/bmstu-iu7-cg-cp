#include <fractals/mandelbulb/Mandelbulb.h>

#include <cmath>

namespace CGCP::fractal {

    float Mandelbulb::raycast(math::Vector3 const &ro, math::Vector3 const &rd, math::Vector4 &rescol, float px) {
        float res = -1.0;

        // bounding sphere
        auto dis = isphere(math::Vector4(0.0, 0.0, 0.0, 1.25), ro, rd);
        if (dis.y() < 0.0) return -1.0;
        dis.setX(std::max(dis.x(), 0.0f));
        dis.setY(std::min(dis.y(), 10.0f));

        // raymarch fractal distance field
        math::Vector4 trap;

        float t = dis.x();
        for (int i = 0; i < 128; i++) {
            auto pos = ro + rd * t;
            float th = 0.25 * px * t;
            float h = map(pos, trap);
            if (t > dis.y() || h < th) break;
            t += h;
        }

        if (t < dis.y()) {
            rescol = trap;
            res = t;
        }

        return res;
    }

    math::Vector3 Mandelbulb::calcNormal(math::Vector3 &pos, float t, float px) {
        math::Vector4 tmp;
        // auto e = math::Vector2(1.0, -1.0) * 0.5773 * 0.25 * px;
        auto e = math::Vector2(1.0, -1.0) * 0.5773 * 2 * px;
        auto e_xyy = math::Vector3(e.x(), e.y(), e.y()),
             e_yyx = math::Vector3(e.y(), e.y(), e.x()),
             e_yxy = math::Vector3(e.y(), e.x(), e.y()),
             e_xxx = math::Vector3(e.x(), e.x(), e.x());
        return math::Vector3(
                       e_xyy * map(pos + e_xyy, tmp) +
                       e_yyx * map(pos + e_yyx, tmp) +
                       e_yxy * map(pos + e_yxy, tmp) +
                       e_xxx * map(pos + e_xxx, tmp))
                .normalized();
    }

    float Mandelbulb::softshadow(math::Vector3 const &ro, math::Vector3 const &rd, float k) {
        float res = 1.0;
        float t = 0.0;
        for (int i = 0; i < 64; i++) {
            math::Vector4 kk;
            float h = map(ro + rd * t, kk);
            res = std::min(res, k * h / t);
            if (res < 0.001) break;
            t += std::clamp(h, 0.01f, 0.2f);
        }
        return std::clamp(res, 0.0f, 1.0f);
    }

    float Mandelbulb::map(math::Vector3 p, math::Vector4 &resColor) {
        math::Vector3 w = p;
        float m = math::Vector3::dotProduct(w, w);

        auto trap = math::Vector4(std::abs(w.x()), std::abs(w.y()), std::abs(w.z()), m);
        float dz = 1.0;

        for (int i = 0; i < 4; i++) {
#if 0
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

    math::Vector2 Mandelbulb::isphere(math::Vector4 const &sph, math::Vector3 const &ro, math::Vector3 const &rd) {
        auto oc = ro - math::Vector3(sph.x(), sph.y(), sph.z());

        float b = math::Vector3::dotProduct(oc, rd);
        float c = math::Vector3::dotProduct(oc, oc) - sph.w() * sph.w();

        float h = b * b - c;

        if (h < 0.0) return math::Vector2(-1.0, 0);

        h = sqrt(h);

        return math::Vector2(-h - b, h - b);
    }

}// namespace CGCP::fractal
