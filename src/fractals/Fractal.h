#pragma once

#include <memory>
#include <vector>

#include <math/Vec3D.h>
#include <math/Ellipse3D.h>

namespace CGCP::fractal {
    class Fractal {
    private:
        using Points = std::vector<math::Ellipse3Df>;
        using PointsPtr = std::shared_ptr<Points>;
        PointsPtr points_;
        math::Vec3Df origin_;

    public:
        Fractal() : points_(std::make_shared<std::vector<math::Ellipse3Df>>()){};
        explicit Fractal(PointsPtr points) : points_(points){};
        Fractal(std::initializer_list<math::Ellipse3Df> items);
        Fractal(PointsPtr points, const math::Vec3Df &origin) : points_(points), origin_(origin){};

        const Points &points() const { return *points_; };
        Points &points() { return *points_; };

        const math::Vec3Df &origin() const { return origin_; };
        math::Vec3Df &origin() { return origin_; };

        ~Fractal() = default;
    };
} // namespace CGCP::fractal