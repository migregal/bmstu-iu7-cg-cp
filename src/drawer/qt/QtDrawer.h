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

        virtual void setFractal(const std::shared_ptr<fractal::Fractal> fractal) override;
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

        void drawFractal();
        QPointF transform(const math::Vec3Df &p);

        QVector2D isphere(QVector4D const &sph, QVector3D const &ro, QVector3D const &rd);

        float raycast(QVector3D const &ro, QVector3D const &rd, QVector4D &rescol, float px);

        QVector3D calcNormal(QVector3D &pos, float t, float px);

        float softshadow(QVector3D const &ro, QVector3D const &rd, float k);
    };
}// namespace CGCP::drawer