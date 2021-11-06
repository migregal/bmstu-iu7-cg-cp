#include <engine/Engine.h>

#include <fractals/julia/Julia.h>
#include <fractals/mandelbulb/Mandelbulb.h>

namespace CGCP {
    Engine::Engine() : camera_(std::make_unique<CameraSolution>()),
                       drawer_(std::make_unique<drawer::DrawerSolution>()),
                       fractal_(std::make_unique<fractal::FractalSolution>()),
                       light_(std::make_unique<light::LightSolution>()) {
        camera().add("main", [=]() -> std::unique_ptr<Camera> {
            return std::make_unique<Camera>();
        });
        fractal().add("mandelbulb parametrized", [=]() -> std::unique_ptr<fractal::Fractal> {
            return std::make_unique<fractal::JuliaParametrized>();
        });
        fractal().add("mandelbulb", [=]() -> std::unique_ptr<fractal::Fractal> {
            return std::make_unique<fractal::Mandelbulb>();
        });
        light().add("ambient", []() -> std::unique_ptr<light::LightSource> {
            return std::make_unique<light::LightSource>(
                    math::Vector3(),
                    0.4,
                    light::LightType::Ambient);
        });
        light().add("directed", []() -> std::unique_ptr<light::LightSource> {
            return std::make_unique<light::LightSource>(
                    math::Vector3(10, 10, -10),
                    0.7,
                    light::LightType::Directional);
        });
    };
}// namespace CGCP
