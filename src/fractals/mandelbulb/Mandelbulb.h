#pragma once

#include <memory>
#include <vector>

#include <fractals/Fractal.h>

namespace CGCP::fractal {
    class Mandelbulb : public Fractal {
    public:
        Mandelbulb() = default;

        float raycast(QVector3D const &ro, QVector3D const &rd, QVector4D &rescol, float px) override;

        ~Mandelbulb() override = default;

        QVector3D calcNormal(QVector3D &pos, float t, float px) override;

        float softshadow(QVector3D const &ro, QVector3D const &rd, float k) override;

    private:
        QVector2D isphere(QVector4D const &sph, QVector3D const &ro, QVector3D const &rd);

        float map(QVector3D p, QVector4D &resColor);
    };
}// namespace CGCP::fractal