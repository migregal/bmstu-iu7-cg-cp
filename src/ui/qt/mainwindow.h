#pragma once

#include <QPixmap>
#include <QProgressDialog>
#include <QShortcut>
#include <QStack>
#include <QtWidgets/QMainWindow>

#include <qt/ui_design.h>

#include <qt/widgets/ColorPicker.h>

#include <engine/Engine.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

signals:
    void drawer_progress(std::shared_ptr<CGCP::drawer::Image> image, double percent, int64_t time);

    void cancel_drawer();

private slots:
    void handle_drawer_progress(std::shared_ptr<CGCP::drawer::Image> image, double percent, int64_t time);

    void handle_cancel_drawer();

protected:
    void update_scene(std::function<void()> cancel_callback);

protected:
    void on_apply_clicked();

    void on_rotation_apply_clicked();

    void on_translation_apply_clicked();

    void on_light_applied();

    void on_ctrl_z_pressed();

    void on_ctrl_shift_z_pressed();

    void on_fractal_changed(int i);

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene_;

    QProgressDialog *dialog_;

    QLabel *time_label_, *scene_resolution_label_;

    ColorPicker *picker_;

    QMetaObject::Connection dialog_conn;

    std::unique_ptr<CGCP::Engine> engine_;

    QStack<QMatrix4x4> stack_backward_, stack_forward_;

    QShortcut *keyCtrlZ, *keyCtrlShiftZ;
};
