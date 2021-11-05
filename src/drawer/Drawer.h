#pragma once

#include <memory>

#include <QImage>

#include <math/Matrix.h>
#include <math/Vector.h>

#include <math/Vector.h>

#include <camera/Camera.h>
#include <fractals/Fractal.h>
#include <light/Light.h>

namespace CGCP::drawer {
    using LightsList = std::vector<std::shared_ptr<light::LightSource>>;
    struct DrawingArgs {
        const std::shared_ptr<Camera> camera;
        const std::shared_ptr<LightsList> lights;
        const std::shared_ptr<fractal::Fractal> fractal;
        const math::Vector3 &color;
        const bool approx;
    };

    using Image = QImage;
    using Pixmap = QPixmap;

    class Drawer {
    public:
        using ProgressCallback = std::function<void(
                std::shared_ptr<Image> image, double percent)>;

        virtual void setFractal(
                const DrawingArgs &args,
                ProgressCallback callback) {
            camera_ = args.camera;
            lights_ = args.lights;
            fractal_ = args.fractal;
            color_ = args.color;
            approx_ = args.approx;
        };

        virtual void cancel() = 0;

        virtual void setImage(const std::shared_ptr<Image> image) = 0;

        virtual ~Drawer() = default;

    protected:
        math::Vector3 render(
                math::Vector2 const &p,
                math::Matrix4x4 const &cam);

        double computeLighting(
                const math::Vector3 &point,
                const math::Vector3 &normal,
                const math::Vector3 &view,
                float fov,
                math::Vector3 const &c);

        virtual math::Vector2 getScreenPos(const math::Vector2 &point);

        virtual float getPx(float fle);

    protected:
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<LightsList> lights_;
        std::shared_ptr<fractal::Fractal> fractal_;
        math::Vector3 color_;
        bool approx_;
    };
}// namespace CGCP::drawer
