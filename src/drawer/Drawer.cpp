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
            color = color_;

            auto pos = ro + t * rd;

            auto nor = fractal_->calcNormal(pos, t, px);
            nor = refVector(nor, -rd);

            double lighting = computeLighting(pos, nor, -1. * rd, px);
            color *= lighting;

            // auto light = math::Vector3(10, 10, -10).normalized();
            // color *= math::Vector3::dotProduct(nor, light);
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
            const float px) {
        auto specular = 1000;

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
            double t_max;

            if (light->getType() == light::LightType::Point) {
                vec_l = light->getPosition() - point;
                t_max = 1.0;
            } else {
                vec_l = light->getPosition();
                t_max = std::numeric_limits<double>::max();
            }

            math::Vector4 tra;
            if (fractal_->raycast(point, vec_l, tra, px) > 0) continue;

            // diffuse reflection
            double n_dot_l = math::Vector3::dotProduct(normal, vec_l);
            if (n_dot_l > 0)
                intensity += light->getIntensity() * n_dot_l /
                             (length_n * vec_l.length());

            // specular reflection
            auto vec_r = normal * (2.f * math::Vector3::dotProduct(normal, vec_l)) - vec_l;
            double r_dot_v = math::Vector3::dotProduct(vec_r, view);
            if (r_dot_v > 0)
                intensity += light->getIntensity() *
                             std::pow(r_dot_v / (vec_r.length() * length_v), specular);
        }

        return intensity;
    }

    math::Vector2 Drawer::getScreenPos(const math::Vector2 &point) {
        return point;
    }

    float Drawer::getPx(float fle) {
        return 2. / (1. * fle);
    }

}// namespace CGCP::drawer
