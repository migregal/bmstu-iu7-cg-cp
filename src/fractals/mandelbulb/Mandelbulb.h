#pragma once

#include <memory>
#include <vector>

#include <fractals/Fractal.h>

namespace CGCP::fractal {
    class Mandelbulb : public Fractal {
    public:
        Mandelbulb() = default;

        virtual float map(QVector3D p, QVector4D &resColor) override;

        ~Mandelbulb() override = default;
    };
}// namespace CGCP::fractal