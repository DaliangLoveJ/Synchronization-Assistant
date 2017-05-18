#include "windows/mainmenu.h"
#include <QAction>
#include <QPixmap>

MainMenu::MainMenu(QWidget *parent)
    : QMenu(parent)
{
    actionSetting = new QAction(this);
    actionAbout = new QAction(this);
    actionSetting->setIcon(QPixmap(":/title/setting"));
    actionAbout->setIcon(QPixmap(":/title/about"));
    addAction(actionSetting);
    addAction(actionAbout);
    connect(actionSetting, SIGNAL(triggered()), this, SIGNAL(showSettingDlg()));
    connect(actionAbout, SIGNAL(triggered()), this, SIGNAL(showAbout()));

    translateLanguage();
}

MainMenu::~MainMenu()
{
}

void MainMenu::translateLanguage()
{
    actionSetting->setText(tr("设置"));
    actionAbout->setText(tr("关于"));
}

