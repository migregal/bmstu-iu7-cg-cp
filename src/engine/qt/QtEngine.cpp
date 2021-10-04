#include <engine/qt/QtEngine.h>

#include <drawer/qt/QtDrawer.h>

namespace CGCP
{
    QtEngine::QtEngine(QGraphicsScene *scene) : Engine()
    {
        drawer().add("main", [=]() -> std::unique_ptr<drawer::Drawer> { return std::make_unique<drawer::QtDrawer>(scene); });
    };
} // namespace CGCP