#include "dayperiod.h"
#include <iostream>
using namespace std;

DayPeriod::DayPeriod(int day)
{
    dayNum = day;

    listItem = new QListWidgetItem();
    listItem->setTextAlignment(Qt::AlignHCenter);
}

DayPeriod::~DayPeriod(){
    delete listItem;
}

void DayPeriod::setInfo(QString &title, QString &condition, QString &temperature){
    this->listItem->setText(title + "\n" + condition + "\n" + temperature);
}


void DayPeriod::setLoading(){
    listItem->setIcon(QIcon(":/loading.png"));
}

void DayPeriod::setIcon(QIcon* icon){
    listItem->setIcon(*icon);
}
