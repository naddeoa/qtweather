#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
	QString* zipCode;
	QString* getZip();

public slots:
    void setZip();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
