#ifndef SETUPATTACKERDIALOG_H
#define SETUPATTACKERDIALOG_H

#include <QDialog>
#include "ui_setupattackerdialog.h"

namespace Ui {
class setupAttackerDialog;
}

class setupAttackerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setupAttackerDialog(QWidget *parent = nullptr);
    ~setupAttackerDialog();

public:
    Ui::setupAttackerDialog *ui;
};

#endif
