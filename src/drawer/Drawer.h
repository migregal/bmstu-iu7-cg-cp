#pragma once

#include <memory>

#include <QImage>

#include <fractals/Fractal.h>
#include <math/Vec3D.h>

namespace CGCP::drawer {
    using Image = QImage;

    class Drawer {
    public:
        using ProgressCallback = std::function<void(std::shared_ptr<Image> image, double percent)>;

        virtual void setFractal(const std::shared_ptr<fractal::Fractal> fractal, ProgressCallback callback) { fractal_ = fractal; };
        virtual void cancel() = 0;

        virtual void setImage(const std::shared_ptr<Image> image) = 0;

        virtual void rotate(const math::Vec3Df &axis, double phi) = 0;
        virtual void translate(const math::Vec3Df &offset) = 0;
        virtual void scale(const math::Vec3Df &scale) = 0;

        virtual ~Drawer() = default;

    protected:
        std::shared_ptr<fractal::Fractal> fractal_;
    };
}// namespace CGCP::drawer