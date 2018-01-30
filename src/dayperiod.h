#ifndef DAYPERIOD_H
#define DAYPERIOD_H
#include <QListWidgetItem>

class DayPeriod : public QObject
{
    Q_OBJECT

public:
    DayPeriod(int day);
    ~DayPeriod();
    void setInfo(QString &title, QString &condition, QString &temperature);
    void setIcon(QIcon* icon);
	int dayNum;
	void setLoading();
    QListWidgetItem* getItem(){
        return listItem;
	}

private:
    QListWidgetItem* listItem;
};

#endif // DAYPERIOD_H
