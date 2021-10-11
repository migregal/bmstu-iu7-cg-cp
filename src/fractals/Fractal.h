#pragma once

#include <memory>
#include <vector>

#include <math/Ellipse3D.h>
#include <math/Vec3D.h>

#include <QVector3D>
#include <QVector4D>

namespace CGCP::fractal {
    using Points = std::vector<math::Ellipse3Df>;
    using PointsPtr = std::shared_ptr<Points>;

    class Fractal {
    public:
        Fractal() = default;

        virtual float map(QVector3D p, QVector4D &resColor) = 0;

        virtual ~Fractal() = default;
    };
}// namespace CGCP::fractal