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
    dialog_->close();

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene_resolution_label_ = new QLabel("Разрешение сцены:");
    ui->statusbar->addPermanentWidget(scene_resolution_label_);
    scene_resolution_label_ = new QLabel();
    ui->statusbar->addPermanentWidget(scene_resolution_label_);

    time_label_ = new QLabel("Время отрисовки:");
    ui->statusbar->addPermanentWidget(time_label_);
    time_label_ = new QLabel();
    ui->statusbar->addPermanentWidget(time_label_);

    picker_ = new ColorPicker(this, {120, 120, 120}, "Выбор цвета");

    ui->color_frame->layout()->addWidget(picker_);

    engine_ = std::make_unique<CGCP::QtEngine>(scene_);

    for (const auto &key : engine_->fractal().getKeys()) {
        ui->fractalComboBox->addItem(key.c_str());
    }

    qRegisterMetaType<std::shared_ptr<CGCP::drawer::Image>>(
            "std::shared_ptr<CGCP::drawer::Image>");
    qRegisterMetaType<int64_t>("int64_t");

    keyCtrlZ = new QShortcut(this);
    keyCtrlZ->setKey(Qt::CTRL + Qt::Key_Z);
    connect(
            keyCtrlZ,
            &QShortcut::activated,
            this,
            &MainWindow::on_ctrl_z_pressed);

    keyCtrlShiftZ = new QShortcut(this);
    keyCtrlShiftZ->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);
    connect(
            keyCtrlShiftZ,
            &QShortcut::activated,
            this,
            &MainWindow::on_ctrl_shift_z_pressed);

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

void MainWindow::on_apply_clicked() {
    update_scene(nullptr);
}

void MainWindow::on_rotation_apply_clicked() {
    stack_forward_.clear();
    stack_backward_.push(engine_->camera().get("main")->getCamera());
    engine_->camera().get("main")->rotate(
            {
                    (float) ui->rotationX->value(),
                    (float) ui->rotationY->value(),
                    (float) ui->rotationZ->value(),
            },
            ui->rotationAlpha->value());

    update_scene([this]() -> void {
        engine_->camera().get("main")->setCamera(stack_backward_.pop());
    });
}

void MainWindow::on_translation_apply_clicked() {
    stack_forward_.clear();
    stack_backward_.push(engine_->camera().get("main")->getCamera());
    engine_->camera().get("main")->translate(
            {
                    (float) ui->translationX->value(),
                    (float) ui->translationY->value(),
                    (float) ui->translationZ->value(),
            });

    update_scene([this]() -> void {
        engine_->camera().get("main")->setCamera(stack_backward_.pop());
    });
}

void MainWindow::on_ctrl_z_pressed() {
    if (stack_backward_.isEmpty()) return;

    stack_forward_.push(engine_->camera().get("main")->getCamera());
    engine_->camera().get("main")->setCamera(stack_backward_.pop());

    update_scene([=]() -> void {
        stack_backward_.push(engine_->camera().get("main")->getCamera());
        engine_->camera().get("main")->setCamera(stack_forward_.pop());
    });
}

void MainWindow::on_ctrl_shift_z_pressed() {
    if (stack_forward_.isEmpty()) return;

    stack_backward_.push(engine_->camera().get("main")->getCamera());
    engine_->camera().get("main")->setCamera(stack_forward_.pop());

    update_scene([=]() -> void {
        stack_forward_.push(engine_->camera().get("main")->getCamera());
        engine_->camera().get("main")->setCamera(stack_backward_.pop());
    });
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    auto rcontent = ui->graphicsView->contentsRect();
    scene_resolution_label_->setText(
            QString::number(rcontent.width()) +
            ";" +
            QString::number(rcontent.height()));
}

void MainWindow::update_scene(std::function<void()> cancel_callback) {
    auto rcontent = ui->graphicsView->contentsRect();
    scene_->setSceneRect(0, 0, rcontent.width(), rcontent.height());

    dialog_->reset();
    dialog_->show();

    auto lights = std::make_shared<CGCP::drawer::LightsList>();
    for (const auto &key : engine_->light().getKeys()) {
        lights->push_back(engine_->light().get(key));
    }

    auto color = picker_->color();
    auto fractal = engine_->fractal().get(
            ui->fractalComboBox->currentText().toStdString());
    engine_->drawer().get("main")->setFractal(
            {engine_->camera().get("main"),
             lights,
             fractal,
             {(float) color.red() / 255.f,
              (float) color.green() / 255.f,
              (float) color.blue() / 255.f},
             ui->approximateFractal->isChecked(),
             (float) ui->alphaSpinBox->value()},
            [=](std::shared_ptr<CGCP::drawer::Image> image,
                double percent,
                int64_t time) -> void {
                static std::atomic_bool first_cancel = true;
                if (!dialog_->wasCanceled()) {
                    emit drawer_progress(image, percent, time);
                    return;
                }

                if (first_cancel && cancel_callback) {
                    first_cancel = false;
                    cancel_callback();
                }
            });
}

void MainWindow::handle_drawer_progress(
        std::shared_ptr<CGCP::drawer::Image> image,
        double percent,
        int64_t time) {
    if (dialog_->wasCanceled()) return;

    dialog_->setValue(percent * 100);

    if (percent >= 1 - std::numeric_limits<double>::epsilon())
        dialog_->close();

    if (image) {
        scene_->clear();
        engine_->drawer().get("main")->setImage(image);
        time_label_->setText(QString::number(time, 'e', 2) + " [µs]");
    }
}

void MainWindow::handle_cancel_drawer() {
    engine_->drawer().get("main")->cancel();
}
