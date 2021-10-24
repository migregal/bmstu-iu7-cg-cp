#include <engine/Engine.h>

#include <fractals/mandelbulb/Mandelbulb.h>

namespace CGCP {
    Engine::Engine() : camera_(std::make_unique<CameraSolution>()),
                       drawer_(std::make_unique<drawer::DrawerSolution>()),
                       fractal_(std::make_unique<fractal::FractalSolution>()) {
        camera().add("main", [=]() -> std::unique_ptr<Camera> {
            return std::make_unique<Camera>();
        });
        fractal().add("mandelbulb", [=]() -> std::unique_ptr<fractal::Fractal> {
            return std::make_unique<fractal::Mandelbulb>();
        });
    };
}// namespace CGCP