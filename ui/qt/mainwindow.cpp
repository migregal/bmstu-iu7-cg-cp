#include <qt/mainwindow.h>

#include <iostream>

#include <QFileDialog>
#include <QMessageBox>

#include <engine/qt/QtEngine.h>
#include <fractals/mandelbulb/Mandelbulb.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    dialog_ = new QProgressDialog("Вычисление...", "Отменить", 0, 100, this);
    dialog_->setWindowModality(Qt::WindowModal);

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    engine_ = std::make_unique<CGCP::QtEngine>(scene_);

    connect(
            ui->applyButton,
            &QPushButton::clicked,
            this,
            &MainWindow::on_apply_clicked);

    connect(
            this,
            &MainWindow::drawer_progress,
            this,
            &MainWindow::handle_drawer_progress);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_apply_clicked() {
    update_scene();
}

void MainWindow::update_scene() {
    auto rcontent = ui->graphicsView->contentsRect();
    scene_->setSceneRect(0, 0, rcontent.width(), rcontent.height());
    scene_->clear();

    dialog_->setValue(0);
    dialog_->show();

    auto fractal = engine_->fractal().get("mandelbulb");
    engine_->drawer().get("main")->setFractal(
            fractal,
            [=](std::shared_ptr<CGCP::drawer::Image> image, double percent) -> void {
                emit drawer_progress(image, percent);
            });
}

void MainWindow::handle_drawer_progress(std::shared_ptr<CGCP::drawer::Image> image, double percent) {
    dialog_->setValue(percent * 100);

    if (percent >= 1 - std::numeric_limits<double>::epsilon())
        dialog_->close();

    if (image)
        engine_->drawer().get("main")->setImage(image);
}
