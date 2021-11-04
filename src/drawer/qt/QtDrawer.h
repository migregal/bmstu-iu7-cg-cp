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
        explicit QtDrawer(QGraphicsScene *scene);

        virtual void setFractal(
                const std::shared_ptr<Camera> camera,
                const std::shared_ptr<fractal::Fractal> fractal,
                ProgressCallback callback,
                bool approx) override;

        virtual void cancel() override;

        virtual void setImage(const std::shared_ptr<Image> image) override;

        virtual ~QtDrawer() override = default;

    private:
        QVector3D render(QVector2D const &p, QMatrix4x4 const &cam);

        QGraphicsScene *scene_;

        void drawFractal(
                const std::shared_ptr<Camera> camera,
                ProgressCallback callback,
                bool approx);

        std::atomic_bool cancelled_ = false;
        std::atomic_bool finished_ = true;
        pthread_t run_thread_;
    };
}// namespace CGCP::drawer
