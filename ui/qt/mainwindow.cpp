#include <qt/mainwindow.h>

#include <iostream>

#include <QMetaType>

#include <QFileDialog>
#include <QMessageBox>

#include <engine/qt/QtEngine.h>
#include <fractals/mandelbulb/Mandelbulb.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    dialog_ = new QProgressDialog("Вычисление...", "Отменить", 0, 100, this);
    dialog_->setWindowModality(Qt::WindowModal);
    dialog_->setAutoReset(true);
    dialog_->close();

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    engine_ = std::make_unique<CGCP::QtEngine>(scene_);

    qRegisterMetaType<std::shared_ptr<CGCP::drawer::Image>>("std::shared_ptr<CGCP::drawer::Image>");

    connect(
            ui->buildImageButton,
            &QPushButton::clicked,
            this,
            &MainWindow::on_apply_clicked);

    connect(
            ui->rotationApply,
            &QPushButton::clicked,
            this,
            &MainWindow::on_rotation_apply_clicked);

    connect(
            ui->translationApply,
            &QPushButton::clicked,
            this,
            &MainWindow::on_translation_apply_clicked);

    connect(
            this,
            &MainWindow::drawer_progress,
            this,
            &MainWindow::handle_drawer_progress);

    connect(
            this->dialog_,
            &QProgressDialog::canceled,
            this,
            &MainWindow::handle_cancel_drawer);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_apply_clicked() {
    update_scene();
}

void MainWindow::on_rotation_apply_clicked() {
    engine_->camera().get("main")->rotate(
            {
                    (float) ui->rotationX->value(),
                    (float) ui->rotationY->value(),
                    (float) ui->rotationZ->value(),
            },
            ui->rotationAlpha->value());

    update_scene();
}

void MainWindow::on_translation_apply_clicked() {
    engine_->camera().get("main")->translate(
            {
                    (float) ui->translationX->value(),
                    (float) ui->translationY->value(),
                    (float) ui->translationZ->value(),
            });
    update_scene();
}

void MainWindow::update_scene() {
    auto rcontent = ui->graphicsView->contentsRect();
    scene_->setSceneRect(0, 0, rcontent.width(), rcontent.height());
    scene_->clear();

    dialog_->reset();
    dialog_->show();

    auto fractal = engine_->fractal().get("mandelbulb");
    engine_->drawer().get("main")->setFractal(
            engine_->camera().get("main"),
            fractal,
            [=](std::shared_ptr<CGCP::drawer::Image> image, double percent) -> void {
                if (dialog_->wasCanceled()) return;
                emit drawer_progress(image, percent);
            });
}

void MainWindow::handle_drawer_progress(std::shared_ptr<CGCP::drawer::Image> image, double percent) {
    if (dialog_->wasCanceled()) return;

    dialog_->setValue(percent * 100);

    if (percent >= 1 - std::numeric_limits<double>::epsilon())
        dialog_->close();

    if (image)
        engine_->drawer().get("main")->setImage(image);
}

void MainWindow::handle_cancel_drawer() {
    engine_->drawer().get("main")->cancel();
}
