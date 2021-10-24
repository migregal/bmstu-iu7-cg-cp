#pragma once

#include <memory>

#include <QImage>

#include <camera/Camera.h>
#include <fractals/Fractal.h>

namespace CGCP::drawer {
    using Image = QImage;

    class Drawer {
    public:
        using ProgressCallback = std::function<void(std::shared_ptr<Image> image, double percent)>;

        virtual void setFractal(
                const std::shared_ptr<Camera> camera,
                const std::shared_ptr<fractal::Fractal> fractal,
                ProgressCallback callback) { fractal_ = fractal; };
        virtual void cancel() = 0;

        virtual void setImage(const std::shared_ptr<Image> image) = 0;

        virtual ~Drawer() = default;

    protected:
        std::shared_ptr<fractal::Fractal> fractal_;
    };
}// namespace CGCP::drawer