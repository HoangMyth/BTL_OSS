#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QTableWidget>
#include <QProcess>
#include <QStringList>
#include <QListWidgetItem>
#include <QTimer>
#include <QString>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Kết nối nút với các hàm xử lý
    connect(ui->processesBtn, SIGNAL(clicked()), this, SLOT(showProcessesWidget()));
    connect(ui->performanceBtn, SIGNAL(clicked()), this, SLOT(showPerformanceWidget()));

    killProcess = new QProcess(this);

    showProcessesWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showProcessesWidget()
{
    // Hiển thị trang chứa processesWidget
    ui->stackedWidget->setCurrentIndex(0);

    // Lấy con trỏ đến QTableWidget đã được thêm vào trong tab "Process"
    QTableWidget *tableWidget = ui->tableWidget;


    // Setup table

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"Process ID", "Name", "Status", "RAM", "CPU"});

    // Display process here

    DisplayProcesses();

    // Khởi tạo và cấu hình QTimer
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateTable);

    // Bắt đầu định kỳ cập nhật mỗi 2 giây (2000 ms)
    updateTimer->start(2000);


}

void MainWindow::DisplayProcesses()
{
    QProcess process;
    process.start("ps", QStringList() << "aux");
    process.waitForFinished();

    QByteArray result = process.readAllStandardOutput();
    QString resultString(result);

    QStringList lines = resultString.split('\n', Qt::SkipEmptyParts);

    // Create a list to store processes data
    QList<QList<QTableWidgetItem*>> processesData;

    foreach (const QString &line, lines) {
        QStringList tokens = line.simplified().split(' ', Qt::SkipEmptyParts);

        if (tokens.size() >= 11) {
            QString processId = tokens.at(1);
            QString processName = tokens.at(10);
            QString processStatus = tokens.at(7);
            QString ramUsage = tokens.at(5);
            QString cpuUsage = tokens.at(2);

            // Create a list to store data for each process
            QList<QTableWidgetItem*> rowData;
            rowData << new QTableWidgetItem(processId);
            rowData << new QTableWidgetItem(processName);
            rowData << new QTableWidgetItem(processStatus);
            rowData << new QTableWidgetItem(ramUsage);
            rowData << new QTableWidgetItem(cpuUsage);

            processesData.append(rowData);
        }
    }

    // Sort processesData based on CPU usage
    std::sort(processesData.begin(), processesData.end(), [](const QList<QTableWidgetItem*> &a, const QList<QTableWidgetItem*> &b) {
        return a[4]->text().toDouble() > b[4]->text().toDouble();
    });

    // Populate the table with sorted data
    ui->tableWidget->setRowCount(0); // Clear existing rows

    int row = 0;
    foreach (const QList<QTableWidgetItem*> &rowData, processesData) {
        ui->tableWidget->insertRow(row);

        // Set data in each cell
        for (int col = 0; col < rowData.size(); ++col) {
            ui->tableWidget->setItem(row, col, rowData.at(col)->clone());
        }

        row++;
    }
}

// Slot để cập nhật bảng tiến trình
void MainWindow::updateTable()
{
    // Dừng timer để tránh cập nhật khi đang xử lý dữ liệu
    updateTimer->stop();

    // Cập nhật bảng tiến trình
    DisplayProcesses();

    // Khởi động lại timer
    updateTimer->start();
}

void MainWindow::killProcessById(const QString &processId) {
    if (!killProcess) {
        qDebug() << "killProcess is null!";
        return;
    }

    QStringList args;
    args << "-9" << processId;  // Sử dụng -9 để kill ngay lập tức

    killProcess->start("kill", args);
    killProcess->waitForFinished();

    if (killProcess->exitCode() == 0) {
        // Kill thành công, cập nhật bảng tiến trình
        DisplayProcesses();
        qDebug() << "Process" << processId << "killed successfully.";

        // Hiển thị thông báo thành công
        QMessageBox::information(this, "Success", "Process killed successfully.");
    } else {
        // Kill thất bại
        qDebug() << "Failed to kill process" << processId;

        // Hiển thị thông báo thất bại
        QMessageBox::critical(this, "Error", "Failed to kill the process.");
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString processId = ui->lineEdit->text();
    qDebug() << "Process ID: " << processId;  // Kiểm tra giá trị của processId
    killProcessById(processId);
}


void MainWindow::showPerformanceWidget()
{
    // Hiển thị trang chứa performanceWidget
    ui->stackedWidget->setCurrentIndex(1);
}



