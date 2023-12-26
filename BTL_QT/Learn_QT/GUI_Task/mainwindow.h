#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QProcess>
#include <QMainWindow>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT

public slots:
    void updateTable();


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void DisplayProcesses();

private slots:
    void showProcessesWidget();
    void showPerformanceWidget();

    void on_pushButton_clicked();
private slots:
    void killProcessById(const QString &processId);
private:
    Ui::MainWindow *ui;
    QTimer *updateTimer;
    QProcess *killProcess;
};
#endif // MAINWINDOW_H



