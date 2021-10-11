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

    auto rcontent = ui->graphicsView->contentsRect();
    _scene->setSceneRect(0, 0, rcontent.width(), rcontent.height());

    engine_ = std::make_unique<CGCP::QtEngine>(_scene);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
}

void MainWindow::update_scene() {
    auto fractal = engine_->fractal().get("mandelbulb");
    engine_->drawer().get("main")->setFractal(fractal);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    auto rcontent = ui->graphicsView->contentsRect();
    _scene->setSceneRect(0, 0, rcontent.width(), rcontent.height());

    update_scene();
}
