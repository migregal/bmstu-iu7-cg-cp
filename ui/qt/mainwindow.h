#pragma once

#include <QtWidgets/QMainWindow>

// #include <qt/design.h>
#include <qt/ui_design.h>

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
    ~MainWindow() override;

protected:
    void update_scene();

protected:
    void on_apply_clicked();
    // void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *_scene;
    std::unique_ptr<CGCP::Engine> engine_;
};
