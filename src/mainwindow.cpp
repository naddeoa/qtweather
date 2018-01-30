#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTemporaryFile>
#include <iostream>
#include <QDesktopServices>
#include "errormessage.h"
#include "stackedwindow.h"
#include "settingsdialog.h"
#include "dialog.h"


using namespace std;

/**
  Constructor for our window.
  @param parent The parent of this window. This specific
  window has no parent as it is the top level window
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);

    /*Create the list of day periods that represent the day / night info
      we get from the xml files and add them to the UI*/
    dayPeriods = new QList<DayPeriod*>();
    for(int i = 0; i < 12; i++){
        DayPeriod *dp = new DayPeriod(i);
        dayPeriods->append(dp);
    }

    connect(ui->listWidget, SIGNAL(activated(QModelIndex)), this, SLOT(dayClick(QModelIndex)));

    pg = new ProgressDialog(this);
    needShowList = true;
    iconCache = new QPixmap*[13];
    for(int i = 0; i < 13; i++){
        iconCache[i] = new QPixmap();
    }

	settings = new QSettings("qtweather", "qtweather", this);
    latlon = new QString(); //graph parameter
    zcode = new QString();  //graph parameter
    doc = new QDomDocument("document"); //will be the xml document object
	zipCache = new QString();
	zipCode = new QString();
	mostRecentHome = HOME_1;

    manager = new QNetworkAccessManager(this);
    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setDayIcons(int)));

    /*addapt the application to maemo by providing
      stackable windows and auto rotation*/
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5StackedWindow, true);
    setAttribute(Qt::WA_Maemo5AutoOrientation);
#endif


    /*signal that the UI has finished setting up, and start the
      dialog for getting a zip code from the user*/
	connect(this, SIGNAL(uiReady(QString*)), this, SLOT(changeZip(QString*)), Qt::QueuedConnection);

	if(settings->value("default") == HOME_1){
		emit uiReady(new QString(settings->value("home1", "").toString()));
	}else if(settings->value("default") == HOME_2){
		emit uiReady(new QString(settings->value("home2", "").toString()));
		mostRecentHome = HOME_2;
	}else{
		emit uiReady(0);
	}
}


/**
  Launch the dialog for getting settings from the user.
  This app has settings for default zipcode launching
  at startup time
  */
void MainWindow::setSettings(){
	QVariant home1, home2, defaultHome;
	home1 = settings->value("home1", "");
	home2 = settings->value("home2", "");
	defaultHome = settings->value("default", 0);
	SettingsDialog sd(home1, home2,  defaultHome, this);

    sd.exec();
    sd.deleteLater();
}

/**
  Deconstructor for the window. When this window closes
  the application ends.
  */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
  Launches the default browser. Loads the current city in the NAOO website
  */
void MainWindow::visitNAOO(){
        QString str("http://forecast.weather.gov/zipcity.php?inputstring=" + *zipCode);
    QDesktopServices::openUrl(QUrl(str));
}

/**
  Function for the day buttons. Shows the detailed view of that day
  */
void MainWindow::dayClick(QModelIndex index){
    ui->listWidget->currentItem()->setSelected(false);
    detailWindow(index.row());
}

/**
  Is called when the windows switches from landscape to portrait
  orientation. The function here specifically switches the scroll
  views layout from horizontal to vertical where appropriate
  @param event contains information about the screens new and
  old layout that we can use to check to see what mode it is currently
  in
  */
void MainWindow::resizeEvent(QResizeEvent *event){
    if(event->oldSize().width() > event->size().width()){
        ui->mainLayout->setDirection(QBoxLayout::TopToBottom);
        ui->listWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }else{
        ui->mainLayout->setDirection(QBoxLayout::LeftToRight);
        ui->listWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    }
}


/**
  Sets the QDomDocument 'doc' variable up to be parsed as an XML file.
  @param reply The network reply that contains the reply. It should be a reply
  from the National Weather Service containing an XML file.
  */
void MainWindow::setDomDocument(QByteArray *reply){
	if (!doc->setContent(*reply)) {
		cout << doc->toDocument().toString().toStdString().c_str() << endl;
        cout << "cold not connect set content1" << endl;
        return;
    }
}


/**
  Launches a dialog that prompts the user to enter a zip code
  into a text box and the executes a request to the server for
  an XML file.
  */
void MainWindow::changeZip(QString* zip){
	*zipCache = *zipCode;  //save our zipcode in case something happens
	delete zipCode;

	if(zip != 0){
		zipCode = new QString(*zip);
	}else{
		Dialog *d = new Dialog(this);
		d->exec();
		zipCode = new QString(*d->getZip());
		d->deleteLater();
	}

	if(!zipCode->isEmpty()){
                QString urlString("http://umcs.maine.edu/~naddeoa/qtweather/?zipcode=" + *zipCode); //TODO

		QNetworkReply* zipReply = manager->get(QNetworkRequest(QUrl(urlString)));
		connect(zipReply, SIGNAL(finished()), this, SLOT(gotXML()));

		pg->show();
	}else{
		restoreZip();
		cout << "Dialog Canceled" << endl;
	}
}

/**
  restores the zip code to the most recent valid one
*/
void MainWindow::restoreZip(){
	zipCode = new QString(*zipCache);
}

/**
  slot for initiating a changeZip() to the users other home
  */
void MainWindow::otherHome(){
	if(mostRecentHome == HOME_1 && settings->value("home2", "").toString() != ""){
		emit uiReady(new QString(settings->value("home2", "").toString()));
		mostRecentHome = HOME_2;
	}else if(mostRecentHome == HOME_2 && settings->value("home1", "").toString() != ""){
		emit uiReady(new QString(settings->value("home1", "").toString()));
		mostRecentHome = HOME_1;
	}
}




/**
  Slot for handling the reception of the XML weather file from the NAOO
  server. This slot parses the data and adds them to the interface
  */
void MainWindow::gotXML(){
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	reply->deleteLater();
	QByteArray str(reply->readAll());

	if( !str.startsWith("<error>") ){
		setDomDocument(&str);
		QDomElement docEle = doc->documentElement();
		deleteCache();

		QDomNodeList days = docEle.elementsByTagName("day");

		//request the icon for the static forecast pane
		QNetworkReply* iconReply = manager->get(QNetworkRequest(QUrl(days.at(0).namedItem("icon").toElement().text())));
		connect(iconReply, SIGNAL(finished()), this, SLOT(setFIcon()));
		ui->forecast1->setText(days.at(0).namedItem("wordedForecast").toElement().text());

		/*Sets the icons to the 'loading' symbol*/
		ui->forecastIcon1->setPixmap(QPixmap(":/loading.png"));
		for(int i = 0 ;i < 12; i++){
			dayPeriods->at(i)->setLoading();
		}

		//Set the title of this window to the city and
		//pass the weather data to the day periods and request
		//the icons for each day also
		/*we do one extra iteration to download icon number 12
		  so that when the user clicks the 11th icon, they can see
		  the full day*/
		setWindowTitle(docEle.toElement().attribute("city"));
		ui->title->setText(days.at(0).namedItem("name").toElement().text());
		for(int i = 0; i < 13; i++){
			if(i < 12){
				QString title(days.at(i).namedItem("name").toElement().text());
				QString condition(days.at(i).namedItem("condition").toElement().text());
				QString temp(days.at(i).namedItem("temp").toElement().text());
				dayPeriods->at(i)->setInfo(title, condition, temp);
			}

			QString icon(days.at(i).namedItem("icon").toElement().text());
			QNetworkReply* dayReply = manager->get(QNetworkRequest(QUrl(icon)));
			connect(dayReply, SIGNAL(finished()), mapper, SLOT(map()));
			mapper->setMapping(dayReply, i);
		}

		//add information to the current day (left pane)
//		QString zc("MEZ015");
		QNetworkReply* tempGraphReply = manager->get(QNetworkRequest(QUrl( QUrl::fromPercentEncoding(days.at(0).toElement().elementsByTagName("graph").at(0).toElement().text().toStdString().c_str()) )));
		connect(tempGraphReply, SIGNAL(finished()), this, SLOT(setTempGraph()));
		ui->forecastTemp->setText(days.at(0).namedItem("temp").toElement().text());
		ui->forecastPrecip->setText("Precipitation: " + days.at(0).namedItem("precipitation").toElement().text());

		if(needShowList){
			needShowList = false;
			for(int i = 0; i < 12; i++){
				ui->listWidget->addItem(dayPeriods->at(i)->getItem());
			}
			ui->weekBanner->setText("Upcoming Days");
			ui->forecastLabel->setText("Hourly Temperatures: ");
			colorize(ui->weekBanner);
			colorize(ui->title);
			colorize(ui->forecastLabel);
		}

		pg->hide();
	}else{
		pg->hide();
		ErrorMessage em(zipCode, this);
		em.exec();
		em.deleteLater();
		restoreZip();
	}

}

/**
  Applies the color theme to a label. The color is
  derived from the system theme and implemented as a
  gradient
  */
void MainWindow::colorize(QLabel *label){
	label->setPalette(QApplication::palette().highlight().color());
	QPalette pal = label->palette();

	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, 35));
	linearGrad.setColorAt(0, pal.color(QPalette::Highlight));
	linearGrad.setColorAt(1, Qt::black);
	QBrush brush(linearGrad);

	pal.setBrush(QPalette::Window, brush);;
	label->setPalette(pal);
}

/**
  Slot for setting the static temperature graph
  in the left pane
  */
void MainWindow::setTempGraph(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QPixmap pix;
    pix.loadFromData(reply->readAll());
    ui->tempGraph->setPixmap(pix);
    reply->deleteLater();
}

/**
  Uses the mapping from the SignalMapper to delegate the proper
  replies to their respective days and passes them the downloaded
  icon
  */
void MainWindow::setDayIcons(int dayIndex){
    QSignalMapper* sm = qobject_cast<QSignalMapper*>(sender());
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sm->mapping(dayIndex));
    reply->deleteLater();

    QPixmap* pix = new QPixmap();
    pix->loadFromData(reply->readAll());

	delete iconCache[dayIndex];
	iconCache[dayIndex] = pix;

	if(dayIndex < 12){//there are 11 days, but 12 pictures
		QIcon icn(*pix);
		dayPeriods->at(dayIndex)->setIcon(&icn);
    }
}




/**
  Sets the first static forecast Icon. This is a reply for a network request
  for the image.
  @param reply The network reply that contains the reply. It should be a reply
  from the National Weather Service containing an XML file.
  */
void MainWindow::setFIcon(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QPixmap pix;
    pix.loadFromData(reply->readAll());
    ui->forecastIcon1->setPixmap(pix);
    reply->deleteLater();
}


/**
  Resets the icon cache to store new icons
  */
void MainWindow::deleteCache(){
    for(int i = 0; i < 13; i++){
		delete iconCache[i];
		iconCache[i] = new QPixmap();
    }
}

/**
  Creates a detailed view of the selected day
  and shows it.
  */
void MainWindow::detailWindow(int day){
    //create lists of all of the relevant information
    QDomElement docEle = doc->documentElement();
	QDomNodeList days = docEle.elementsByTagName("day");

    int dayVar = day;
    int nightVar = day;
	days.at(day).namedItem("wordedForecast").toElement().text().contains("night", Qt::CaseInsensitive) ? dayVar = day-1 : nightVar = day+1;

	StackedWindow *sw = new StackedWindow(doc, *iconCache[dayVar],dayVar, *iconCache[nightVar], nightVar, this);
#ifdef Q_WS_MAEMO_5
    sw->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    sw->setAttribute(Qt::WA_DeleteOnClose);
    sw->show();

}
