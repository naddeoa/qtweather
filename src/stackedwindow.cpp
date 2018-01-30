#include "stackedwindow.h"
#include <QPropertyAnimation>
#include "ui_stackedwindow.h"
#include "mainwindow.h"
#include <iostream>
using namespace std;


StackedWindow::StackedWindow(QDomDocument *doc, QPixmap &dayIcon, int dayVar, QPixmap &nightIcon, int nightVar, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StackedWindow)
{
    ui->setupUi(this);
	this->doc = doc;
	QDomElement docEle = doc->documentElement();
	QDomNodeList days = docEle.elementsByTagName("day");
	QPixmap tempPix(1600, 220);


	/*init block*/
	manager = new QNetworkAccessManager(this);
	mapper = new QSignalMapper(this);
	QDomNodeList graphs = days.at(dayVar).toElement().elementsByTagName("graph");
	frameLen = graphs.length();
	frames = new GraphFrame*[graphs.length()];


	//add weather data to the UI
	ui->day->setText(days.at(dayVar).namedItem("name").toElement().text());
	ui->hi->setText(days.at(dayVar).namedItem("temp").toElement().text());
	ui->lo->setText(days.at(nightVar).namedItem("temp").toElement().text());
	ui->precip->setText("Precipitation: " + days.at(dayVar).namedItem("precipitation").toElement().text());
	ui->dayForecast->setText(days.at(dayVar).namedItem("wordedForecast").toElement().text());
	ui->nightForecast->setText(days.at(nightVar).namedItem("wordedForecast").toElement().text());
	MainWindow::colorize(ui->label_3);

	/*add the layouts to the ui*/
	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->frame->layout());
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setIcons(int)));

	/*Creates the graph frames*/
	for(int i = 0; i < graphs.length(); i++){
		QString header = graphs.at(i).toElement().attribute("header");
		GraphFrame* gf = new GraphFrame(header, tempPix);
		frames[i] = gf;
		if(i < graphs.length()-2)
			layout->addLayout(gf->getLayout());
		QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(QUrl::fromPercentEncoding(graphs.at(i).toElement().text().toStdString().c_str()))));
		connect(reply, SIGNAL(finished()), mapper, SLOT(map()));
		mapper->setMapping(reply, i);
	}

	//sends a request for the dayIcon if it's not cahced
	if(!dayIcon.isNull()){
		ui->dayIcon->setPixmap(dayIcon);
	}else{
		ui->dayIcon->setPixmap(QPixmap(":/loading.png"));
		QString dayURL(QUrl::fromPercentEncoding(days.at(dayVar).namedItem("icon").toElement().text().toStdString().c_str()));
		QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(dayURL)));
		connect(reply, SIGNAL(finished()), mapper, SLOT(map()));
		mapper->setMapping(reply, graphs.length());
	}

	//sends a request for the nightIcon if it's not cahced
	if(!nightIcon.isNull()){
		ui->nightIcon->setPixmap(nightIcon);
    }else{
        ui->nightIcon->setPixmap(QPixmap(":/loading.png"));
		QString nightURL(QUrl::fromPercentEncoding(days.at(nightVar).namedItem("icon").toElement().text().toStdString().c_str()));
		QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(nightURL)));
		connect(reply, SIGNAL(finished()), mapper, SLOT(map()));
		mapper->setMapping(reply, graphs.length()+1);
    }


	//add the rain graph if it's applicable. the rain graph is always the second to last graph in the list
	if(ui->dayForecast->text().contains("ain") || ui->nightForecast->text().contains("ain") ||
	   ui->dayForecast->text().contains("hower") || ui->nightForecast->text().contains("hower")){
		layout->addLayout(frames[graphs.length()-2]->getLayout());
	}

	//add the snow graph if it's applicable. the snow graph is always the last graph in the list
	if(ui->dayForecast->text().contains("now") || ui->nightForecast->text().contains("now")){
		layout->addLayout(frames[graphs.length()-1]->getLayout());
	}


}

StackedWindow::~StackedWindow()
{
    delete ui;
	delete manager;
	delete mapper;
	for(int i = 0; i < frameLen; i++){
		delete frames[i];
	}
}


/**
  Sets the icons in this view from the download. This is only
  called if the icons were not cached in the mainwindow
*/
void StackedWindow::setIcons(int mapping){
    QSignalMapper* sm = qobject_cast<QSignalMapper*>(sender());
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sm->mapping(mapping));
	reply->deleteLater();
    QPixmap pix;
    pix.loadFromData(reply->readAll());


	if(mapping < frameLen){
		frames[mapping]->setPix(pix);
	}else if(mapping == frameLen){
		ui->dayIcon->setPixmap(pix);
	}else if(mapping == frameLen+1){
		ui->nightIcon->setPixmap(pix);
	}


}


/**
  Resize the window when switching between landscape and portrait.
  */
void StackedWindow::resizeEvent(QResizeEvent *event){
    if(event->oldSize().width() > event->size().width()){
		ui->frame->setMinimumWidth(100);
    }else{
		ui->frame->setMinimumWidth(720);
    }

}

