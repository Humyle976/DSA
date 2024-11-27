#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QTextEdit>
#include "simulationdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_configureYourServerButton_clicked();

    void on_setupYourAttackerButton_clicked();

    void on_runSimulationButton_clicked();

    void simulateDdosAttack(simulationDialog *Dialog);
private:
    Ui::MainWindow *ui;
};
#endif
