#include "configureserverdialog.h"
#include "ui_configureserverdialog.h"

ConfigureServerDialog::ConfigureServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureServerDialog)
{
    ui->setupUi(this);
}

ConfigureServerDialog::~ConfigureServerDialog()
{
    delete ui;
}


