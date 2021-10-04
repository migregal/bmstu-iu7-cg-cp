#include <QDebug>

#include <drawer/qt/QtDrawer.h>

#define PERSPECTIVE_VERTICAL_ANGLE 90
#define PERSPECTIVE_NEAR_PLANE 0.1
#define PERSPECTIVE_FAR_PLANE 20

namespace CGCP::drawer {
    static inline QVector3D Vec3Df2QVector3D(const math::Vec3Df &v) {
        return QVector3D(v.x(), v.y(), v.z());
    }

    QtDrawer::QtDrawer(QGraphicsScene *scene) : scene_(scene){};

    void QtDrawer::setFractal(const std::shared_ptr<fractal::Fractal> fractal) {
        Drawer::setFractal(fractal);
        drawFractal();
    };

    void QtDrawer::drawFractal() {
        scene_->clear();
    }

    QPointF QtDrawer::transform(const math::Vec3Df &p) {
        return QPointF();
    }

    void QtDrawer::rotate(const math::Vec3Df &axis, double phi) {
        rotate_.rotate(phi, axis.x(), axis.y(), axis.z());
        drawFractal();
    }

    void QtDrawer::translate(const math::Vec3Df &offset) {
        translate_ += Vec3Df2QVector3D(offset);
        drawFractal();
    }

    void QtDrawer::scale(const math::Vec3Df &scale) {
        scale_ *= Vec3Df2QVector3D(scale);
        drawFractal();
    }
}// namespace CGCP::drawer