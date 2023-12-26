#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Kết nối nút với các hàm xử lý
    connect(ui->processesBtn, SIGNAL(clicked()), this, SLOT(showProcessesWidget()));
    connect(ui->performanceBtn, SIGNAL(clicked()), this, SLOT(showPerformanceWidget()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showProcessesWidget()
{
    // Hiển thị trang chứa processesWidget
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showPerformanceWidget()
{
    // Hiển thị trang chứa performanceWidget
    ui->stackedWidget->setCurrentIndex(1);
}

