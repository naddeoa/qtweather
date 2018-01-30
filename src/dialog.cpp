#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->zipCodeEntry->setInputMethodHints(Qt::ImhDigitsOnly);
	zipCode = new QString();
}

Dialog::~Dialog()
{
    delete ui;
	delete zipCode;
}

void Dialog::setZip(){
	*zipCode = QString(ui->zipCodeEntry->text());
}


QString* Dialog::getZip(){
	return zipCode;
}
