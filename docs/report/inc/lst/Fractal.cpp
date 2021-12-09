float Fractal::raymarching(
        math::Vector3 const &ro,
        math::Vector3 const &rd,
        math::Vector4 &rescol,
        float fov,
        math::Vector3 const &c) {
    float res = -1.0;

    auto dis = bounder_->intersect(ro, rd);

    if (dis.y() < 0.0) return -1.0;
    dis.setX(std::max(dis.x(), 0.0f));

    // raymarch fractal distance field
    math::Vector4 trap;

    float fovfactor = 1.0 / sqrt(1.0 + fov * fov);
    float t = dis.x();
    for (int i = 0; i < MAX_RAYMARCH_STEP; i++) {
        auto pos = ro + rd * t;
        float surface = std::clamp(1e-3f * t * fovfactor, 1e-4f, 0.1f);

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
