#ifndef STACKEDWINDOW_H
#define STACKEDWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QSignalMapper>
#include <QPropertyAnimation>
#include <QLabel>
#include <QDomDocument>
#include "graphframe.h"

namespace Ui {
    class StackedWindow;
}

class StackedWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit StackedWindow(QDomDocument* doc, QPixmap& dayIcon, int dayVar, QPixmap& nightIcon, int nightVar, QWidget *parent = 0);
    ~StackedWindow();
//    void setInfo(QString &day, QString &hi, QString &lo, QString &precip, QString &dayFor, QString &nightFor);


protected:
    virtual void resizeEvent(QResizeEvent * event);

private slots:
	void setIcons(int mapping);

private:
    Ui::StackedWindow *ui;
	GraphFrame** frames;
	int frameLen;
    QSignalMapper* mapper;
	QNetworkAccessManager *manager;
	QDomDocument *doc;
};

#endif // STACKEDWINDOW_H
