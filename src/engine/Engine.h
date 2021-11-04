#pragma once

#include <memory>

#include <camera/CameraSolution.h>
#include <drawer/DrawerSolution.h>
#include <fractals/FractalSolution.h>
#include <light/LightSolution.h>

namespace CGCP {
    class Engine {
    public:
        Engine();
        virtual ~Engine() = default;

        drawer::DrawerSolution &drawer() { return *drawer_; };

        fractal::FractalSolution &fractal() { return *fractal_; };

        CameraSolution &camera() { return *camera_; };

        light::LightSolution &light() { return *light_; }

    private:
        std::unique_ptr<CameraSolution> camera_;
        std::unique_ptr<drawer::DrawerSolution> drawer_;
        std::unique_ptr<fractal::FractalSolution> fractal_;
        std::unique_ptr<light::LightSolution> light_;
    };
}// namespace CGCP
