#include "simulationdialog.h"
#include "ui_simulationdialog.h"

simulationDialog::simulationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::simulationDialog)
{
    ui->setupUi(this);
    connect(this, &simulationDialog::appendTerminal, this, &simulationDialog::on_appendTerminal);
    connect(this, &simulationDialog::updateProgress, this, &simulationDialog::on_progressBar_valueChanged);
}

simulationDialog::~simulationDialog()
{
    delete ui;
}

void simulationDialog::on_progressBar_valueChanged(double value)
{
    ui->progressBar->setValue(value);
    if (value >= ui->progressBar->maximum()) {
        ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    } else {
        ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    }
}


void simulationDialog::on_appendTerminal(const QString &message)
{
    ui->Terminal->append(message);
}
