#-------------------------------------------------
#
# Project created by QtCreator 2011-01-15T21:04:57
#
#-------------------------------------------------

QT       += core gui\
            xml\
            network\
            maemo5

TARGET = qtweather
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    stackedwindow.cpp \
    dayperiod.cpp \
    errormessage.cpp \
    progressdialog.cpp \
    settingsdialog.cpp \
    graphframe.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    stackedwindow.h \
    dayperiod.h \
    errormessage.h \
    progressdialog.h \
    settingsdialog.h \
    graphframe.h

FORMS    += mainwindow.ui \
    dialog.ui \
    stackedwindow.ui \
    errormessage.ui \
    progressdialog.ui \
    settingsdialog.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe6277400
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

unix {
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /opt/qtweather
  }
  BINDIR = $$PREFIX/bin
  DATADIR =/usr/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  #MAKE INSTALL

  INSTALLS += target desktop iconxpm icon26 icon48 icon64

  target.path =$$BINDIR

  desktop.path = $$DATADIR/applications/hildon
  desktop.files += ../data/$${TARGET}.desktop

  icon64.path = $$DATADIR/icons/hicolor/64x64/apps
  icon64.files += ../data/64x64/$${TARGET}.png
}

RESOURCES += \
    style.qrc
