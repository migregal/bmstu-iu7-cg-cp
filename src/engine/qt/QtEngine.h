#pragma once

#include <QGraphicsScene>
#include <memory>

#include <engine/Engine.h>

namespace CGCP {
    class QtEngine : public Engine {
    public:
        QtEngine() = delete;
        QtEngine(QGraphicsScene *scene);
        ~QtEngine() override = default;
    };
}// namespace CGCP