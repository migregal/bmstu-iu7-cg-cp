#include <engine/Engine.h>

#include <fractals/mandelbulb/Mandelbulb.h>

namespace CGCP {
    Engine::Engine() : camera_(std::make_unique<CameraSolution>()),
                       drawer_(std::make_unique<drawer::DrawerSolution>()),
                       fractal_(std::make_unique<fractal::FractalSolution>()),
                       light_(std::make_unique<light::LightSolution>()) {
        camera().add("main", [=]() -> std::unique_ptr<Camera> {
            return std::make_unique<Camera>();
        });
        fractal().add("mandelbulb", [=]() -> std::unique_ptr<fractal::Fractal> {
            return std::make_unique<fractal::Mandelbulb>();
        });
        light().add("ambient", []() -> std::unique_ptr<light::LightSource> {
            return std::make_unique<light::LightSource>(math::Vector3(), 0.2, light::LightType::Ambirent);
        });
    };
}// namespace CGCP
