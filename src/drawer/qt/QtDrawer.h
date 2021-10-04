#pragma once

#include <QGraphicsScene>
#include <QMatrix4x4>

#include <drawer/Drawer.h>
#include <fractals/Fractal.h>
#include <math/Vec3D.h>

namespace CGCP::drawer {
    class QtDrawer : public Drawer {
    public:
        QtDrawer() = delete;
        explicit QtDrawer(QGraphicsScene *scene);

        virtual void setFractal(const std::shared_ptr<fractal::Fractal> fractal) override;
        virtual void rotate(const math::Vec3Df &axis, double phi) override;
        virtual void translate(const math::Vec3Df &offset) override;
        virtual void scale(const math::Vec3Df &scale) override;

        virtual ~QtDrawer() override = default;

    private:
        QGraphicsScene *scene_;

        QVector3D scale_{1, 1, 1};
        QVector3D translate_;
        QMatrix4x4 rotate_;
        QMatrix4x4 projective_;

        void drawFractal();
        QPointF transform(const math::Vec3Df &p);
    };
}// namespace CGCP::drawer