#include <algorithm>
#include <cmath>

#include <QDebug>

#include <QPainter>

#include <omp.h>

#include <drawer/qt/QtDrawer.h>

#define PERSPECTIVE_VERTICAL_ANGLE 90
#define PERSPECTIVE_NEAR_PLANE 0.1
#define PERSPECTIVE_FAR_PLANE 20

const QVector3D light1 = QVector3D(0.577, 0.577, -0.577);
const QVector3D light2 = QVector3D(-0.707, 0.000, 0.707);

namespace CGCP::drawer {
    static inline QVector3D Vec3Df2QVector3D(const math::Vec3Df &v) {
        return QVector3D(v.x(), v.y(), v.z());
    }

    QtDrawer::QtDrawer(QGraphicsScene *scene) : scene_(scene){};

    void QtDrawer::setFractal(const std::shared_ptr<fractal::Fractal> fractal) {
        Drawer::setFractal(fractal);
        drawFractal();
    };

    static inline QVector3D mix(QVector3D const &x, QVector3D const &y, float a) {
        return x * (1 - a) + y * a;
    }

    static inline QVector3D refVector(QVector3D v, QVector3D n) {
        return v;
        // float k = QVector3D::dotProduct(v, n);
        // //return (k>0.0) ? v : -v;
        // return (k > 0.0) ? v : v - 2.0 * n * k;
    }

    static inline QVector3D reflect(QVector3D I, QVector3D N) {
        return I - 2.0 * QVector3D::dotProduct(N, I) * N;
    }

    QVector3D QtDrawer::render(QVector2D const &p, QMatrix4x4 const &cam) {
        const float fle = 1.5;

        QVector2D sp = (2.0 * p - QVector2D(scene_->width(), scene_->height())) / scene_->height();
        float px = 2.0 / (scene_->height() * fle);

        auto ro = QVector3D(cam.column(3));
        auto rd = (QVector3D(cam * QVector4D(sp.x(), sp.y(), fle, 0.0))).normalized();

        QVector4D tra;
        float t = fractal_->raycast(ro, rd, tra, px);

        QVector3D color;

        if (t < 0.0) {
            color = (0.6 + 0.4 * rd.y()) * QVector3D(0.8, .9, 1.1);
            color += 5.0 * QVector3D(0.8, 0.7, 0.5) * std::pow(std::clamp(QVector3D::dotProduct(rd, light1), 0.0f, 1.0f), 32.0);
        } else {
            color = QVector3D(0.01, 0.01, 0.01);
            color = mix(color, QVector3D(0.10, 0.20, 0.30), std::clamp(tra.y(), 0.0f, 1.0f));
            color = mix(color, QVector3D(0.02, 0.10, 0.30), std::clamp(tra.z() * tra.z(), 0.0f, 1.0f));
            color = mix(color, QVector3D(0.30, 0.10, 0.02), std::clamp(std::pow(tra.w(), 6.0), 0.0, 1.0));
            color *= 0.5;

            auto pos = ro + t * rd;
            auto nor = fractal_->calcNormal(pos, t, px);

            nor = refVector(nor, -rd);

            auto hal = (light1 - rd).normalized();
            auto ref = reflect(rd, nor);
            float occ = std::clamp(0.05 * std::log(tra.x()), 0.0, 1.0);
            float fac = std::clamp(1.0 + QVector3D::dotProduct(rd, nor), 0.0, 1.0);

            // sun
            float sha1 = fractal_->softshadow(pos + 0.001 * nor, light1, 32.0);
            float dif1 = std::clamp(QVector3D::dotProduct(light1, nor), 0.0f, 1.0f) * sha1;
            float spe1 = std::pow(std::clamp(QVector3D::dotProduct(nor, hal), 0.0f, 1.0f), 32.0) * dif1 * (0.04 + 0.96 * std::pow(std::clamp(1.0f - QVector3D::dotProduct(hal, light1), 0.0f, 1.0f), 5.0));
            // bounce
            float dif2 = std::clamp(0.5f + 0.5f * QVector3D::dotProduct(light2, nor), 0.0f, 1.0f) * occ;
            // sky
            float dif3 = (0.7 + 0.3 * nor.y()) * (0.2 + 0.8 * occ);

            auto lin = QVector3D(0.0, 0.0, 0.0);
            lin += 12.0 * QVector3D(1.50, 1.10, 0.70) * dif1;
            lin += 4.0 * QVector3D(0.25, 0.20, 0.15) * dif2;
            lin += 1.5 * QVector3D(0.10, 0.20, 0.30) * dif3;
            lin += 2.5 * QVector3D(0.35, 0.30, 0.25) * (0.05 + 0.95 * occ);
            lin.setX(lin.x() + 4.0 * fac * occ);
            lin.setY(lin.y() + 4.0 * fac * occ);
            lin.setZ(lin.z() + 4.0 * fac * occ);
            color *= lin;
            color.setX(std::pow(color.x(), 0.7));
            color.setY(std::pow(color.y(), 0.9));
            color.setX(color.x() + spe1 * 15.0);
            color.setY(color.y() + spe1 * 15.0);
            color.setZ(color.z() + spe1 * 15.0);
        }

        color.setX(std::pow(color.x(), 0.4545));
        color.setY(std::pow(color.y(), 0.4545));
        color.setZ(std::pow(color.z(), 0.4545));

        color *= 1.0 - 0.05 * sp.length();

        return color;
    }

    void QtDrawer::drawFractal() {
        scene_->clear();
        // float time = 22.15 * .1;
        float time = 0;

        // camera
        float di = 1.4 + 0.1 * cos(.29 * time);
        auto ro = di * QVector3D(cos(.33 * time), 0.8 * sin(.37 * time), sin(.31 * time));
        auto ta = QVector3D(0.0, 0.1, 0.0);
        float cr = 0.5 * cos(0.1 * time);

        // camera matrix
        auto cp = QVector3D(sin(cr), cos(cr), 0.0);
        auto cw = (ta - ro).normalized();
        auto cu = QVector3D::crossProduct(cw, cp).normalized();
        auto cv = QVector3D::crossProduct(cu, cw);
        auto cam = QMatrix4x4(
                cu.x(), cu.y(), cu.z(), ro.x(),
                cv.x(), cv.y(), cv.z(), ro.y(),
                cw.x(), cw.y(), cw.z(), ro.z(),
                .0, .0, .0, 1.0);

        auto colors = new QColor *[scene_->width()];
#pragma omp parallel for schedule(dynamic)
        for (size_t i = 0; i < scene_->width(); ++i)
            colors[i] = new QColor[scene_->height()];

#pragma omp parallel for collapse(2) schedule(dynamic)
        for (size_t i = 0; i < scene_->width(); ++i) {
            for (size_t j = 0; j < scene_->height(); ++j) {
                QVector3D col = render(QVector2D(i, j), cam);
                if (i == 574 && j == 136)
                    qDebug() << i << " " << j << " "
                             << col.x() << " "
                             << col.y() << " "
                             << col.z() << "\n";
                colors[i][j] = QColor(255 * col.x(), 255 * col.y(), 255 * col.z());
            }
        }

        QImage image(scene_->width(), scene_->height(), QImage::Format_RGB32);
        for (size_t i = 0; i < scene_->width(); ++i)
            for (size_t j = 0; j < scene_->height(); ++j)
                image.setPixelColor(i, j, colors[i][j]);

#pragma omp parallel for
        for (size_t i = 0; i < scene_->width(); ++i)
            delete[] colors[i];
        delete[] colors;

        scene_->clear();
        scene_->addPixmap(QPixmap::fromImage(image));
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