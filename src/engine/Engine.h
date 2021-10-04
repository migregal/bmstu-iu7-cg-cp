#pragma once

#include <memory>

#include <drawer/DrawerSolution.h>

namespace CGCP {
    class Engine {
    public:
        Engine() : drawer_(std::make_unique<drawer::DrawerSolution>()){};

        drawer::DrawerSolution &drawer() { return *drawer_; };

        virtual ~Engine() = default;
    private:
        std::unique_ptr<drawer::DrawerSolution> drawer_;
    };
}