#pragma once

#include <fractals/Fractal.h>

namespace CGCP::fractal {
    void Fractal::setBounder(std::shared_ptr<bounder::Bounder> bounder) {
        bounder_ = bounder;
    }
}// namespace CGCP::fractal
