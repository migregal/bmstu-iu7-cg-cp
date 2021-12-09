double Drawer::computeLighting(
        const math::Vector3 &point,
        const math::Vector3 &normal,
        const math::Vector3 &view,
        float fov,
        math::Vector3 const &c) {
    if (!lights_) return 0.0;

    double intensity = 0;
    auto length_n = normal.length();
    auto length_v = view.length();

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
        auto n_dot_l = math::Vector3::dotProduct(normal, vec_l);
        if (n_dot_l > 0)
            intensity += fractal_->getRoughness() *
                light->getIntensity() * n_dot_l /
                (length_n * vec_l.length());

        // specular reflection
        auto vec_r = normal * (2.f * n_dot_l) - vec_l;
        auto r_dot_v = math::Vector3::dotProduct(vec_r, view);
        if (r_dot_v > 0) {
            intensity += light->getIntensity() *
                std::pow(r_dot_v / (vec_r.length() * length_v),
                SPECULARITY);
        }
    }

    return intensity;
}
