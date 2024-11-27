#ifndef SIMULATIONDIALOG_H
#define SIMULATIONDIALOG_H

#include <QDialog>
#include <QString>
#include "ui_simulationdialog.h"

namespace Ui {
class simulationDialog;
}

class simulationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit simulationDialog(QWidget *parent = nullptr);
    ~simulationDialog();

signals:
    void updateProgress(double);
    void appendTerminal(const QString&);
    void stopSimulation();

public slots:
    void on_progressBar_valueChanged(double value);
    void on_appendTerminal(const QString &message);

public:
    Ui::simulationDialog *ui;
};

#endif
