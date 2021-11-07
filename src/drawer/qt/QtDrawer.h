#pragma once

#include <QGraphicsScene>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector4D>

#include <drawer/Drawer.h>
#include <fractals/Fractal.h>

namespace CGCP::drawer {
    class QtDrawer : public Drawer {
    public:
        QtDrawer() = delete;

        ~QtDrawer() override;

        explicit QtDrawer(QGraphicsScene *scene);

        virtual void setFractal(
                const DrawingArgs &args,
                ProgressCallback callback) override;

        virtual void cancel() override;

        virtual void setImage(const std::shared_ptr<Image> image) override;

    protected:
        math::Vector2 getScreenPos(const math::Vector2 &point) override;

    private:
        void drawFractal(ProgressCallback callback);

        QGraphicsScene *scene_;

        std::atomic_bool cancelled_ = false;
        std::atomic_bool finished_ = true;
        pthread_t run_thread_;
    };
}// namespace CGCP::drawer
