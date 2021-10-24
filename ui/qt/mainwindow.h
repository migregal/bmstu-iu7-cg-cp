#pragma once

#include <QProgressDialog>
#include <QtWidgets/QMainWindow>

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

signals:
    void drawer_progress(std::shared_ptr<CGCP::drawer::Image> image, double percent);

private slots:
    void handle_drawer_progress(std::shared_ptr<CGCP::drawer::Image> image, double percent);

protected:
    void update_scene();

protected:
    void on_apply_clicked();
    // void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene_;
    QProgressDialog *dialog_;

    std::unique_ptr<CGCP::Engine> engine_;
};
