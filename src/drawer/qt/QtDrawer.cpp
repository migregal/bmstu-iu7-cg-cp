#include <thread>

#include <QDebug>

#include <QPainter>

#include <omp.h>

#include <drawer/qt/QtDrawer.h>

#define APPROX_SQUARE_SIDE 3.
#define APPROX_SQUARE_SQUARE 9.

namespace CGCP::drawer {
    QtDrawer::QtDrawer(QGraphicsScene *scene) : scene_(scene){};

    void QtDrawer::setFractal(
            const DrawingArgs &args,
            ProgressCallback callback) {
        if (!finished_) return;
        finished_ = false;

        Drawer::setFractal(args, callback);

        std::thread thr(&QtDrawer::drawFractal, std::ref(*this), callback);
        run_thread_ = thr.native_handle();
        thr.detach();
    };

    void QtDrawer::setImage(const std::shared_ptr<Image> image) {
        if (!image)
            return;
        scene_->addPixmap(Pixmap::fromImage(*image));
    }

    math::Vector2 QtDrawer::getScreenPos(const math::Vector2 &point) {
        return (2.0 * point - QVector2D(scene_->width(), scene_->height())) /
               scene_->height();
    }

    float QtDrawer::getPx(float fle) {
        return 2.0 / (scene_->height() * fle);
    }

    void QtDrawer::drawFractal(const ProgressCallback callback) {
        std::shared_ptr<Image> result;

        auto colors = new QColor *[scene_->width()];
        for (size_t i = 0; i < scene_->width(); ++i)
            colors[i] = new QColor[scene_->height()];

        auto stop = false;
        size_t count = 0, max = scene_->width() * scene_->height() + 1;

#pragma omp parallel for collapse(2) schedule(dynamic)
        for (size_t i = 0; i < scene_->width(); ++i) {
            for (size_t j = 0; j < scene_->height(); ++j) {
                if (cancelled_) continue;

                if (!approx_) {
                    QVector3D col = render(QVector2D(i, scene_->height() - j), camera_->getCamera());
                    colors[i][j] = QColor(255 * col.x(), 255 * col.y(), 255 * col.z());
                } else {
                    QVector3D col;
                    for (int k = 0; k < APPROX_SQUARE_SIDE; k++)
                        for (int l = 0; l < APPROX_SQUARE_SIDE; l++)
                            col += render(
                                    QVector2D(i, scene_->height() - j) +
                                            (QVector2D(l, k) / APPROX_SQUARE_SIDE),
                                    camera_->getCamera());

                    colors[i][j] = QColor(
                            (255. * col.x()) / APPROX_SQUARE_SQUARE,
                            (255. * col.y()) / APPROX_SQUARE_SQUARE,
                            (255. * col.z()) / APPROX_SQUARE_SQUARE);
                }
#pragma omp critical
                callback(result, (double(++count) / max));
            }
        }

        if (!cancelled_) {
            result = std::make_shared<QImage>(scene_->width(), scene_->height(), QImage::Format_RGB32);
            for (size_t i = 0; i < scene_->width(); ++i)
                for (size_t j = 0; j < scene_->height(); ++j)
                    result->setPixelColor(i, j, colors[i][j]);
        }

        for (size_t i = 0; i < scene_->width(); ++i)
            delete[] colors[i];
        delete[] colors;

        finished_ = true;

        callback(result, 1);
    }

    void QtDrawer::cancel() {
        if (finished_)
            return;

        cancelled_ = true;

        // wait 3 seconds before cancel thread using system
        for (int i = 30; !finished_ && i > 0; i--) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (!finished_) {
            pthread_cancel(run_thread_);
            finished_ = true;
        }

        cancelled_ = false;
    };
}// namespace CGCP::drawer
