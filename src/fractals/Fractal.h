#pragma once

#include <memory>
#include <vector>

#include <math/Ellipse3D.h>
#include <math/Vec3D.h>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

namespace CGCP::fractal {
    using Points = std::vector<math::Ellipse3Df>;
    using PointsPtr = std::shared_ptr<Points>;

    class Fractal {
    public:
        Fractal() = default;

        virtual float raycast(QVector3D const &ro, QVector3D const &rd, QVector4D &rescol, float px) = 0;

        virtual QVector3D calcNormal(QVector3D &pos, float t, float px) = 0;

        virtual float softshadow(QVector3D const &ro, QVector3D const &rd, float k) = 0;

        virtual ~Fractal() = default;
    };
}// namespace CGCP::fractal