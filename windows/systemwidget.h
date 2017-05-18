#ifndef SYSTEMWIDGET_H
#define SYSTEMWIDGET_H
#include <QWidget>
#include <QDebug>
#include <QHostInfo>
#include <QLabel>
#include <QNetworkInterface>
#include <QSettings>
#include <QCoreApplication>
#include <QDnsHostAddressRecord>
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>
#include <QDir>
#include "functions/common.h"
#include "windows/messagebox.h"
class QPushButton;

class SystemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SystemWidget(QWidget *parent = 0);
    ~SystemWidget();
    void system_onekey(void);
public:
    QPushButton *sysExportBtn;
signals:
    void onekeyexportSys();

protected:
    void paintEvent(QPaintEvent *);
    void _init(void);
public slots:
    void exportSysSlot();
    void compressData();

private:
    QLabel *ipContentLabel;
    QLabel *maskContentLabel;
    QLabel *gatewayContentLabel;
    QLabel *dnsContentLabel;
    CustomMessageBox *msg;
};

#endif // SYSTEMWIDGET_H

