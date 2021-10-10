#pragma once

#include <memory>

#include <drawer/DrawerSolution.h>
#include <fractals/FractalSolution.h>

namespace CGCP {
    class Engine {
    public:
        Engine();

        drawer::DrawerSolution &drawer() { return *drawer_; };
        fractal::FractalSolution &fractal() { return *fractal_; };
        virtual ~Engine() = default;

    private:
        std::unique_ptr<drawer::DrawerSolution> drawer_;
        std::unique_ptr<fractal::FractalSolution> fractal_;
    };
}// namespace CGCP