#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>

SettingsDialog::SettingsDialog(QVariant& home1, QVariant& home2, QVariant& defaultHome, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
	ui->home1Entry->setText(home1.toString());
    ui->home1Entry->setInputMethodHints(Qt::ImhDigitsOnly);
	ui->home2Entry->setText(home2.toString());
    ui->home2Entry->setInputMethodHints((Qt::ImhDigitsOnly));
	ui->activeComboBox->setCurrentIndex(defaultHome.toInt());

    connect(this, SIGNAL(accepted()), this, SLOT(setValues()));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

/** Relay the layouts when going between portrait and landscape */
void SettingsDialog::resizeEvent(QResizeEvent *event){
	if(event->oldSize().width() > event->size().width()){
		ui->mainLayout->setDirection(QBoxLayout::TopToBottom);
	}else{
		ui->mainLayout->setDirection(QBoxLayout::LeftToRight);
	}
}

/** Write the new values to the settings flie*/
void SettingsDialog::setValues(){
	QSettings* settings = new QSettings("qtweather", "qtweather", this);
	settings->setValue("home1", ui->home1Entry->text());
	settings->setValue("home2", ui->home2Entry->text());
	settings->setValue("default", ui->activeComboBox->currentIndex());
	settings->deleteLater();
}
