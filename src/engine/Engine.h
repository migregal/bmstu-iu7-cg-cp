#pragma once

#include <memory>

#include <camera/CameraSolution.h>
#include <drawer/DrawerSolution.h>
#include <fractals/FractalSolution.h>

namespace CGCP {
    class Engine {
    public:
        Engine();
        virtual ~Engine() = default;

        drawer::DrawerSolution &drawer() { return *drawer_; };

        fractal::FractalSolution &fractal() { return *fractal_; };

        CameraSolution &camera() { return *camera_; };

    private:
        std::unique_ptr<CameraSolution> camera_;
        std::unique_ptr<drawer::DrawerSolution> drawer_;
        std::unique_ptr<fractal::FractalSolution> fractal_;
    };
}// namespace CGCP