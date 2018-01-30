#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QResizeEvent>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
	explicit SettingsDialog(QVariant& home1, QVariant& home2, QVariant& defaultHome, QWidget *parent = 0);
    ~SettingsDialog();

protected:
	virtual void resizeEvent(QResizeEvent * event);


private:
    Ui::SettingsDialog *ui;

private slots:
    void setValues();

};

#endif // SETTINGSDIALOG_H
