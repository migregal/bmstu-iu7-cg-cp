#include <drawer/Drawer.h>

#include <algorithm>
#include <cmath>
#include <limits>

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

    static inline math::Vector3 refovct(math::Vector3 I, math::Vector3 N) {
        return I - 2.0 * math::Vector3::dotProduct(N, I) * N;
    }

    static inline math::Vector3 getCC(double alpha_) {
        auto cc = math::Vector3(0.9 * cos(3.9 + 1.2 * alpha_) - .3,
                                0.8 * cos(2.5 + 1.1 * alpha_),
                                0.8 * cos(3.4 + 1.3 * alpha_));
        if (cc.length() < 0.50) cc = 0.50 * cc.normalized();
        if (cc.length() > 0.95) cc = 0.95 * cc.normalized();

        return cc;
    }

    math::Vector3 Drawer::render(math::Vector2 const &pos, math::Matrix4x4 const &cam) {
        const float fov = 1.5;

        auto cc = getCC(alpha_);

        QVector2D sp = getScreenPos(pos);

        auto ro = math::Vector3(cam.column(3));
        auto rd = math::Vector3(cam * math::Vector4(sp.x(), sp.y(), fov, 0.0)).normalized();

        math::Vector4 tra;
        float t = fractal_->raycast(ro, rd, tra, fov, cc);

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
            color = fractal_->getColor();
            // color = mix(color, {0.10, 0.20, 0.30}, std::clamp(tra.y(), 0.0f, 1.0f));
            // color = mix(color, {0.02, 0.30, 0.10}, std::clamp(tra.z() * tra.z(), 0.0f, 1.0f));
            // color = mix(color, {0.30, 0.10, 0.02}, std::clamp(pow(tra.w(), 6.0), 0.0, 1.0));

            auto pos = ro + t * rd;

            auto nor = fractal_->calcNormal(pos, t, fov, cc);
            nor = refVector(nor, -rd);

            double lighting = computeLighting(pos, nor, -1. * rd, fov, cc);
            color *= lighting;
        }

        return math::Vector3(
                std::clamp(color.x(), 0.0f, 1.0f),
                std::clamp(color.y(), 0.0f, 1.0f),
                std::clamp(color.z(), 0.0f, 1.0f));
    }

    double Drawer::computeLighting(
            const math::Vector3 &point,
            const math::Vector3 &normal,
            const math::Vector3 &view,
            float fov,
            math::Vector3 const &c) {
        if (!lights_) return 0.0;

        double intensity = 0;
        double length_n = normal.length();
        double length_v = view.length();

        for (const auto &light : *lights_) {
            if (light->getType() == light::LightType::Ambient) {
                intensity += light->getIntensity();
                continue;
            }

            auto vec_l = math::Vector3();
            double t_max = 1.0;

            if (light->getType() == light::LightType::Point) {
                vec_l = light->getPosition() - point;
            } else {
                vec_l = light->getPosition();
                t_max = std::numeric_limits<double>::max();
            }

            math::Vector4 tra;
            if (fractal_->raycast(point, vec_l, tra, fov, c) > 0) continue;

            // diffuse reflection
            double n_dot_l = math::Vector3::dotProduct(normal, vec_l);
            if (n_dot_l > 0)
                intensity += fractal_->getRoughness() *
                             light->getIntensity() * n_dot_l /
                             (length_n * vec_l.length());

            // specular reflection
            auto vec_r = normal * (2.f * n_dot_l) - vec_l;
            double r_dot_v = math::Vector3::dotProduct(vec_r, view);
            if (r_dot_v > 0) {
                intensity += light->getIntensity() *
                             std::pow(r_dot_v / (vec_r.length() * length_v),
                                      1 + 99 * (1 - fractal_->getSpecular()));
            }
        }

        return intensity;
    }

    math::Vector2 Drawer::getScreenPos(const math::Vector2 &point) {
        return point;
    }
}// namespace CGCP::drawer
