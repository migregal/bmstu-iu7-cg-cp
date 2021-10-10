#pragma once

#include <memory>
#include <vector>

#include <fractals/Fractal.h>

namespace CGCP::fractal {
    class Mandelbulb : public Fractal {
    public:
        Mandelbulb();
        // Fractal() : points_(std::make_shared<std::vector<math::Ellipse3Df>>()){};
        // explicit Fractal(PointsPtr points) : points_(points){};
        // Fractal(std::initializer_list<math::Ellipse3Df> items);
        // Fractal(PointsPtr points, const math::Vec3Df &origin) : points_(points), origin_(origin){};

        const Points &points() const override;
        Points &points() override;

        ~Mandelbulb() override = default;
    };
}// namespace CGCP::fractal