#pragma once

#include <memory>
#include <vector>

#include <math/Ellipse3D.h>
#include <math/Vec3D.h>

namespace CGCP::fractal {
    using Points = std::vector<math::Ellipse3Df>;
    using PointsPtr = std::shared_ptr<Points>;

    class Fractal {
    public:
        Fractal() : points_(std::make_shared<std::vector<math::Ellipse3Df>>()){};
        explicit Fractal(PointsPtr points) : points_(points){};
        Fractal(std::initializer_list<math::Ellipse3Df> items);
        Fractal(PointsPtr points, const math::Vec3Df &origin) : points_(points), origin_(origin){};

        virtual const Points &points() const { return *points_; };
        virtual Points &points() { return *points_; };

        const math::Vec3Df &origin() const { return origin_; };
        math::Vec3Df &origin() { return origin_; };

        virtual ~Fractal() = default;

    private:
        PointsPtr points_;
        math::Vec3Df origin_;
    };
}// namespace CGCP::fractal