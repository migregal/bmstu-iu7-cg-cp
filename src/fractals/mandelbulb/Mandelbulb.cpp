#include <fractals/mandelbulb/Mandelbulb.h>

namespace CGCP::fractal {
    Mandelbulb::Mandelbulb() : Fractal() {}

    const Points &Mandelbulb::points() const {
        return Fractal::points();
    }

    Points &Mandelbulb::points() {
        return Fractal::points();
    }
}// namespace CGCP::fractal