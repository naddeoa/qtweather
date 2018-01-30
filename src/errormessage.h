#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QDialog>

namespace Ui {
    class ErrorMessage;
}

class ErrorMessage : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorMessage(QString *zip, QWidget *parent = 0);
    ~ErrorMessage();

public slots:
    void visitSite();

private:
    Ui::ErrorMessage *ui;
    QString *zip;
};

#endif // ERRORMESSAGE_H
