#include <qt/mainwindow.h>

#include <iostream>

#include <QFileDialog>
#include <QMessageBox>

#include <engine/qt/QtEngine.h>
#include <fractals/mandelbulb/Mandelbulb.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(_scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    engine_ = std::make_unique<CGCP::QtEngine>(_scene);

    connect(ui->applyButton, &QPushButton::clicked, this,
            &MainWindow::on_apply_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_apply_clicked() {
    update_scene();
}

void MainWindow::update_scene() {
    auto rcontent = ui->graphicsView->contentsRect();
    _scene->setSceneRect(0, 0, rcontent.width(), rcontent.height());

    auto fractal = engine_->fractal().get("mandelbulb");
    engine_->drawer().get("main")->setFractal(fractal);
}