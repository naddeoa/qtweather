#include "errormessage.h"
#include "ui_errormessage.h"
#include <QDesktopServices>
#include <QUrl>

ErrorMessage::ErrorMessage(QString *zipcode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorMessage)
{
    zip = zipcode;
    ui->setupUi(this);
}


ErrorMessage::~ErrorMessage()
{
    delete ui;
}

/** Use Qt's desktop services to open weather url in the default browser*/
void ErrorMessage::visitSite(){
    QString str("http://forecast.weather.gov/zipcity.php?inputstring=" + *zip);
    QDesktopServices::openUrl(QUrl(str));
}
