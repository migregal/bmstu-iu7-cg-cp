#include <algorithm>

#include <QDebug>

#include <QVector2D>
#include <QVector4D>

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

    float map(QVector3D p, QVector4D &resColor) {
        QVector3D w = p;
        float m = QVector3D::dotProduct(w, w);

        auto trap = QVector4D(std::abs(w.x()), std::abs(w.y()), std::abs(w.z()), m);
        float dz = 1.0;

        for (int i = 0; i < 4; i++) {
#if 1
            // polynomial version (no trigonometrics, but MUCH slower)
            float m2 = m * m;
            float m4 = m2 * m2;
            dz = 8.0 * std::sqrt(m4 * m2 * m) * dz + 1.0;

            float x = w.x();
            float x2 = x * x;
            float x4 = x2 * x2;
            float y = w.y();
            float y2 = y * y;
            float y4 = y2 * y2;
            float z = w.z();
            float z2 = z * z;
            float z4 = z2 * z2;

            float k3 = x2 + z2;
            float k2 = 1 / std::sqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
            float k1 = x4 + y4 + z4 - 6.0 * y2 * z2 - 6.0 * x2 * y2 + 2.0 * z2 * x2;
            float k4 = x2 - y2 + z2;

            w.setX(p.x() + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2);
            w.setY(p.y() + -16.0 * y2 * k3 * k4 * k4 + k1 * k1);
            w.setZ(p.z() + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4 + z4 * z4) * k1 * k2);
#else
            // trigonometric version (MUCH faster than polynomial)

            // dz = 8*z^7*dz
            dz = 8.0 * pow(m, 3.5) * dz + 1.0;
            //dz = 8.0*pow(sqrt(m),7.0)*dz + 1.0;

            // z = z^8+z
            float r = length(w);
            float b = 8.0 * acos(w.y / r);
            float a = 8.0 * atan(w.x, w.z);
            w = p + pow(r, 8.0) * vec3(sin(b) * sin(a), cos(b), sin(b) * cos(a));
#endif
            auto cmp = QVector4D(std::abs(w.x()), std::abs(w.y()), std::abs(w.z()), m);
            if (trap.length() > cmp.length())
                trap = cmp;

            m = QVector3D::dotProduct(w, w);
            if (m > 256.0)
                break;
        }

        resColor = QVector4D(m, trap.y(), trap.z(), trap.w());

        // distance estimation (through the Hubbard-Douady potential)
        return 0.25 * log(m) * sqrt(m) / dz;
    }

    QVector2D isphere(QVector4D const &sph, QVector3D const &ro, QVector3D const &rd) {
        auto oc = ro - QVector3D(sph.x(), sph.y(), sph.z());
        float b = QVector3D::dotProduct(oc, rd);
        float c = QVector3D::dotProduct(oc, oc) - sph.w() * sph.w();
        float h = b * b - c;
        if (h < 0.0) return QVector2D(-1.0, 0);
        h = sqrt(h);
        return QVector2D(-h - b, h - b);
    }

    static float raycast(QVector3D const &ro, QVector3D const &rd, QVector4D &rescol, float px) {
        float res = -1.0;

        // bounding sphere
        auto dis = isphere(QVector4D(0.0, 0.0, 0.0, 1.25), ro, rd);
        if (dis.y() < 0.0) return -1.0;
        dis.setX(std::max(dis.x(), 0.0f));
        dis.setY(std::min(dis.y(), 10.0f));

        // raymarch fractal distance field
        QVector4D trap;

        float t = dis.x();
        for (int i = 0; i < 128; i++) {
            auto pos = ro + rd * t;
            float th = 0.25 * px * t;
            float h = map(pos, trap);
            if (t > dis.y() || h < th) break;
            t += h;
        }

        if (t < dis.y()) {
            rescol = trap;
            res = t;
        }

        return res;
    }

    static inline QVector3D mix(QVector3D const &x, QVector3D const &y, float a) {
        return x * (1 - a) + y * a;
    }

    static inline QVector3D refVector(QVector3D v, QVector3D n) {
        return v;
        // float k = dot(v, n);
        // //return (k>0.0) ? v : -v;
        // return (k > 0.0) ? v : v - 2.0 * n * k;
    }

    QVector3D calcNormal(QVector3D &pos, float t, float px) {
        QVector4D tmp;
        auto e = QVector2D(1.0, -1.0) * 0.5773 * 0.25 * px;
        auto e1 = QVector3D(e.x(), e.y(), e.y()), e2 = QVector3D(e.y(), e.y(), e.x()),
             e3 = QVector3D(e.y(), e.x(), e.y()), e4 = QVector3D(e.x(), e.x(), e.x());
        return QVector3D(
                       e1 * map(pos + e1, tmp) +
                       e2 * map(pos + e2, tmp) +
                       e3 * map(pos + e3, tmp) +
                       e4 * map(pos + e4, tmp))
                .normalized();
    }

    static inline QVector3D reflect(QVector3D I, QVector3D N) {
        return I - 2.0 * QVector3D::dotProduct(N, I) * N;
    }

    float softshadow(QVector3D const &ro, QVector3D const &rd, float k) {
        float res = 1.0;
        float t = 0.0;
        for (int i = 0; i < 64; i++) {
            QVector4D kk;
            float h = map(ro + rd * t, kk);
            res = std::min(res, k * h / t);
            if (res < 0.001) break;
            t += std::clamp(h, 0.01f, 0.2f);
        }
        return std::clamp(res, 0.0f, 1.0f);
    }

    QVector3D QtDrawer::render(QVector2D const &p, QMatrix4x4 const &cam) {
        const float fle = 1.5;

        QVector2D sp = (2.0 * p - QVector2D(scene_->width(), scene_->height())) / scene_->height();
        float px = 2.0 / (scene_->height() * fle);

        auto ro = QVector3D(cam.column(3));
        auto rd = (QVector3D(cam * QVector4D(sp.x(), sp.y(), fle, 0.0))).normalized();

        QVector4D tra;
        float t = raycast(ro, rd, tra, px);

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
            auto nor = calcNormal(pos, t, px);

            nor = refVector(nor, -rd);

            auto hal = (light1 - rd).normalized();
            auto ref = reflect(rd, nor);
            float occ = std::clamp(0.05 * log(tra.x()), 0.0, 1.0);
            float fac = std::clamp(1.0 + QVector3D::dotProduct(rd, nor), 0.0, 1.0);

            // sun
            float sha1 = softshadow(pos + 0.001 * nor, light1, 32.0);
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

        // col = pow(col, QVector3D(0.4545, 0.4545, 0.4545));
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
        for (size_t i = 0; i < scene_->width(); ++i)
            colors[i] = new QColor[scene_->height()];

#pragma omp parallel for collapse(2)
        for (size_t i = 0; i < scene_->width(); ++i) {
            for (size_t j = 0; j < scene_->height(); ++j) {
                QVector3D col = render(QVector2D(i, j), cam);
                colors[i][j] = QColor(255 * col.x(), 255 * col.y(), 255 * col.z());
            }
        }

        QImage image(scene_->width(), scene_->height(), QImage::Format_ARGB32_Premultiplied);

        auto qpen = QPen({0, 0, 0});
        QPainter qPainter(&image);
        qPainter.setBrush(Qt::NoBrush);
        qPainter.setPen(qpen);

        for (size_t i = 0; i < scene_->width(); ++i) {
            for (size_t j = 0; j < scene_->height(); ++j) {
                qpen.setColor(colors[i][j]);
                qPainter.setPen(qpen);
                qPainter.drawPoint(i, j);
            }
        }

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