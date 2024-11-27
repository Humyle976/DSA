#include "setupattackerdialog.h"
#include "ui_setupattackerdialog.h"

setupAttackerDialog::setupAttackerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::setupAttackerDialog)
{
    ui->setupUi(this);
}

setupAttackerDialog::~setupAttackerDialog()
{
    delete ui;
}
