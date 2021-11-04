#include <drawer/Drawer.h>

#include <algorithm>
#include <cmath>

namespace CGCP::drawer {
    const auto light1 = math::Vector3(0.577, 0.577, -0.577);
    const auto light2 = math::Vector3(-0.707, 0.000, 0.707);

    static inline math::Vector3 mix(math::Vector3 const &x, math::Vector3 const &y, float a) {
        return x * (1 - a) + y * a;
    }

    static inline math::Vector3 refVector(math::Vector3 d, math::Vector3 n) {
        // r = d - 2*(d*n)*n
        float k = math::Vector3::dotProduct(d, n);
        return (k > 0.0) ? d : d - 2.0 * n * k;
    }

    static inline math::Vector3 reflect(math::Vector3 I, math::Vector3 N) {
        return I - 2.0 * math::Vector3::dotProduct(N, I) * N;
    }

    math::Vector3 Drawer::render(math::Vector2 const &p, math::Matrix4x4 const &cam) {
        const float fle = 1.5;

        QVector2D sp = getScreenPos(p);
        float px = getPx(fle);

        auto ro = math::Vector3(cam.column(3));
        auto rd = math::Vector3(cam * math::Vector4(sp.x(), sp.y(), fle, 0.0)).normalized();

        math::Vector4 tra;
        float t = fractal_->raycast(ro, rd, tra, px);

        math::Vector3 color;

        if (t < 0.0) {
            color = (0.6 + 0.4 * rd.y()) * math::Vector3(0.8, .9, 1.1);
            color += 5.0 * math::Vector3(0.8, 0.7, 0.5) *
                     std::pow(
                             std::clamp(math::Vector3::dotProduct(rd, light1),
                                        0.0f,
                                        1.0f),
                             32.0);
        } else {
            color = math::Vector3(0.01, 0.01, 0.01);
            color = mix(color, math::Vector3(0.10, 0.20, 0.30), std::clamp(tra.y(), 0.0f, 1.0f));
            color = mix(color, math::Vector3(0.02, 0.10, 0.30), std::clamp(tra.z() * tra.z(), 0.0f, 1.0f));
            color = mix(color, math::Vector3(0.30, 0.10, 0.02), std::clamp(std::pow(tra.w(), 6.0), 0.0, 1.0));
            color *= 0.5;

            auto pos = ro + t * rd;
            auto nor = fractal_->calcNormal(pos, t, px);
            nor = refVector(nor, -rd);

            auto hal = (light1 - rd).normalized();
            auto ref = reflect(rd, nor);
            float occ = std::clamp(0.05 * std::log(tra.x()), 0.0, 1.0);
            float fac = std::clamp(1.0 + math::Vector3::dotProduct(rd, nor), 0.0, 1.0);

            // sun
            float sha1 = fractal_->softshadow(pos + 0.001 * nor, light1, 32.0);
            float dif1 = std::clamp(math::Vector3::dotProduct(light1, nor), 0.0f, 1.0f) * sha1;
            float spe1 = std::pow(std::clamp(math::Vector3::dotProduct(nor, hal), 0.0f, 1.0f), 32.0) * dif1 * (0.04 + 0.96 * std::pow(std::clamp(1.0f - math::Vector3::dotProduct(hal, light1), 0.0f, 1.0f), 5.0));
            // bounce
            float dif2 = std::clamp(0.5f + 0.5f * math::Vector3::dotProduct(light2, nor), 0.0f, 1.0f) * occ;
            // sky
            float dif3 = (0.7 + 0.3 * nor.y()) * (0.2 + 0.8 * occ);

            auto lin = math::Vector3(0.0, 0.0, 0.0);
            lin += 12.0 * math::Vector3(1.50, 1.10, 0.70) * dif1;
            lin += 4.0 * math::Vector3(0.25, 0.20, 0.15) * dif2;
            lin += 1.5 * math::Vector3(0.10, 0.20, 0.30) * dif3;
            lin += 2.5 * math::Vector3(0.35, 0.30, 0.25) * (0.05 + 0.95 * occ);
            lin.setX(lin.x() + 4.0 * fac * occ);
            lin.setY(lin.y() + 4.0 * fac * occ);
            lin.setZ(lin.z() + 4.0 * fac * occ);
            color *= lin;
            color.setX(std::pow(color.x(), 0.7));
            color.setY(std::pow(color.y(), 0.9));
            color.setX(color.x() + spe1 * 15.0);
            color.setY(color.y() + spe1 * 15.0);
            color.setZ(color.z() + spe1 * 15.0);
        }

        color.setX(std::pow(color.x(), 0.4545));
        color.setY(std::pow(color.y(), 0.4545));
        color.setZ(std::pow(color.z(), 0.4545));

        color *= 1.0 - 0.05 * sp.length();

        return math::Vector3(
                std::clamp(color.x(), 0.0f, 1.0f),
                std::clamp(color.y(), 0.0f, 1.0f),
                std::clamp(color.z(), 0.0f, 1.0f));
    }

    double Drawer::computeLighting(
            const math::Vector3 &point,
            const math::Vector3 &normal,
            const math::Vector3 &view) {
        if (!lights_) return 0.0;

        double intensity = 0;
        double length_n = normal.length();
        double length_v = view.length();

        for (const auto &light : *lights_) {
        }

        return 0.0;
    }

    math::Vector2 Drawer::getScreenPos(const math::Vector2 &point) {
        return point;
    }

    float Drawer::getPx(float fle) {
        return 2. / (1. * fle);
    }

}// namespace CGCP::drawer
