#pragma once

#include <QGraphicsScene>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector4D>

#include <drawer/Drawer.h>
#include <fractals/Fractal.h>
#include <math/Vec3D.h>

namespace CGCP::drawer {
    class QtDrawer : public Drawer {
    public:
        QtDrawer() = delete;
        explicit QtDrawer(QGraphicsScene *scene);

        virtual void setFractal(const std::shared_ptr<fractal::Fractal> fractal, ProgressCallback callback) override;

        virtual void cancel() override;

        virtual void setImage(const std::shared_ptr<Image> image) override;

        virtual void rotate(const math::Vec3Df &axis, double phi) override;
        virtual void translate(const math::Vec3Df &offset) override;
        virtual void scale(const math::Vec3Df &scale) override;

        virtual ~QtDrawer() override = default;

    private:
        QVector3D render(QVector2D const &p, QMatrix4x4 const &cam);

        QGraphicsScene *scene_;

        QVector3D scale_{1, 1, 1};
        QVector3D translate_;
        QMatrix4x4 rotate_;
        QMatrix4x4 projective_;

        void drawFractal(ProgressCallback callback);

        QPointF transform(const math::Vec3Df &p);

        std::atomic_bool cancelled_ = false;
        std::atomic_bool finished_ = true;
        pthread_t run_thread_;
    };
}// namespace CGCP::drawer