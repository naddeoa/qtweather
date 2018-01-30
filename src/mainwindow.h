#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomDocument>
#include <QSignalMapper>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QResizeEvent>
#include <QLabel>
#include <QUrl>
#include <QSettings>
#include "progressdialog.h"
#include "stackedwindow.h"
#include "dayperiod.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
	static const int HOME_1 = 1;
	static const int HOME_2 = 2;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	static void colorize(QLabel* label);

protected:
    virtual void resizeEvent(QResizeEvent * event);

signals:
	void uiReady(QString* zipcode = 0);

private:
    Ui::MainWindow *ui;
    QSettings* settings;
    QDomDocument *doc;
    QNetworkAccessManager *manager;
    QSignalMapper* mapper;
	QString* zipCode;
	QString* zipCache;
    QString *latlon;
    QString *zcode;
    QList<DayPeriod*>* dayPeriods;
    QPixmap** iconCache; //array of icon pointers
    QIcon *icon;
    bool needShowList;
	int mostRecentHome;
    ProgressDialog *pg;

    void detailWindow(int day);
	void setDomDocument(QByteArray* reply);
	void deleteCache();
	void restoreZip();


private slots:
    void gotXML();
    void setDayIcons(int dayIndex);
    void setFIcon();
    void setTempGraph();

    void dayClick(QModelIndex index);
	void changeZip(QString* zipcode = 0);
    void setSettings();
	void visitNAOO();
	void otherHome();


};

#endif // MAINWINDOW_H
