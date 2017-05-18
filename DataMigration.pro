#-------------------------------------------------
#
# Project created by QtCreator 2017-01-03T18:17:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataMigration
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x



SOURCES += main.cpp\
    windows/datamigration.cpp \
    tools/utils.cpp \
    windows/pushbutton.cpp \
    windows/toolbutton.cpp \
    windows/mainmenu.cpp \
    windows/systemtray.cpp \
    windows/computerwidget.cpp \
    windows/sysinfolabel.cpp \
    windows/progresslabel.cpp \
    windows/aboutus.cpp \
    windows/settingdialog.cpp \
    windows/userwidget.cpp \
    windows/appwidget.cpp \
    windows/systemwidget.cpp \
    windows/userdetailwidget.cpp \
    windows/appdetailwidget.cpp \
    windows/userdatamodel.cpp \
    windows/titlewidget.cpp \
    functions/createconfigure.cpp \
    windows/directorydlg.cpp \
    functions/browserdata.cpp \
    functions/emaildata.cpp \
    windows/comboboxdelegate.cpp \
    windows/customdelegate.cpp \
    windows/QRoundProgressBar.cpp \
    windows/messagebox.cpp \
    functions/userdata.cpp \
    windows/networktrans.cpp \
    windows/ftpmanager.cpp

HEADERS  += \
    windows/datamigration.h \
    tools/utils.h \
    functions/email.h \
    windows/pushbutton.h \
    windows/toolbutton.h \
    windows/mainmenu.h \
    windows/systemtray.h \
    windows/computerwidget.h \
    windows/sysinfolabel.h \
    windows/progresslabel.h \
    windows/aboutus.h \
    windows/settingdialog.h \
    windows/userwidget.h \
    windows/appwidget.h \
    windows/systemwidget.h \
    windows/userdetailwidget.h \
    windows/appdetailwidget.h \
    windows/userdatamodel.h \
    windows/titlewidget.h \
    functions/common.h \
    functions/createconfigure.h \
    windows/directorydlg.h \
    functions/browserdata.h \
    functions/emaildata.h \
    windows/comboboxdelegate.h \
    windows/customdelegate.h \
    windows/QRoundProgressBar.h \
    windows/messagebox.h \
    functions/userdata.h \
    windows/networktrans.h \
    tools/ftpmanager.h

RESOURCES += \
    migration.qrc


LIBS += -lIphlpapi


LIBS += -L$$PWD/quazip/lib  -lquazipd
INCLUDEPATH += F:/DataMigration/quazip/include
DEPENDPATH  += F:/DataMigration/quazip/include
