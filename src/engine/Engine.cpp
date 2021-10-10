#include <engine/Engine.h>

#include <fractals/mandelbulb/Mandelbulb.h>

namespace CGCP {
    Engine::Engine() : drawer_(std::make_unique<drawer::DrawerSolution>()),
                       fractal_(std::make_unique<fractal::FractalSolution>()) {
        fractal().add("mandelbulb", [=]() -> std::unique_ptr<fractal::Fractal> {
            return std::make_unique<fractal::Mandelbulb>();
        });
    };
}// namespace CGCP