#pragma once

#include <memory>

#include <fractals/Fractal.h>
#include <math/Vec3D.h>

namespace CGCP::drawer {
    class Drawer {
    private:
    protected:
        std::shared_ptr<fractal::Fractal> fractal_;

    public:
        virtual void setFractal(const std::shared_ptr<fractal::Fractal> fractal) { fractal_ = fractal; };
        virtual void rotate(const math::Vec3Df &axis, double phi) = 0;
        virtual void translate(const math::Vec3Df &offset) = 0;
        virtual void scale(const math::Vec3Df &scale) = 0;

        virtual ~Drawer() = default;
    };
}// namespace CGCP