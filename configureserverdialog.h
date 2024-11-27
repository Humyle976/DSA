#ifndef CONFIGURESERVORDIALOG_H
#define CONFIGURESERVORDIALOG_H

#include <QDialog>
#include "ui_configureserverdialog.h"

namespace Ui {
class ConfigureServerDialog;
}

class ConfigureServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureServerDialog(QWidget *parent = nullptr);
    ~ConfigureServerDialog();

public:
    Ui::ConfigureServerDialog *ui;
};

#endif
