#include "windows/systemtray.h"
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QApplication>

SystemTray::SystemTray(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QPixmap(":/title/dlogo"));
    trayMenu = new QMenu();
    actionOpen = new QAction(this);
    actionExit = new QAction(this);
    trayMenu->addAction(actionOpen);
    trayMenu->addAction(actionExit);
    connect(actionOpen, SIGNAL(triggered(bool)), this, SIGNAL(showWidget()));
    connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    setContextMenu(trayMenu);
    translateLanguage();
}

SystemTray::~SystemTray()
{
}

void SystemTray::translateLanguage()
{
    setToolTip(tr("数据迁移"));
    actionOpen->setText(tr("打开"));
    actionExit->setText(tr("退出"));
}
