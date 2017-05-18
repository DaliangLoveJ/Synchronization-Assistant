#include "windows/datamigration.h"
#include "windows/messagebox.h"
#include "windows/networktrans.h"
#include "functions/emaildata.h"
#include "functions/browserdata.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QStackedWidget>
#include <QListWidget>
#include <QMessageBox>

#include "quazip.h"
#include "JlCompress.h"

DataMigration::DataMigration(QWidget *parent)
    : QWidget(parent)
{
    resize(920, 576);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QPixmap(":/title/logo"));
    dstFilePath = Util::readDstFileFromIni();
    titleWidget = new TitleWidget();
    mainMenu = new MainMenu(this);
    systemTray = new SystemTray(this);
    stackWidget = new QStackedWidget();
    compuWidget = new ComputerWidget();
    aboutUs = new AboutUsDialog();
    settingDlg = new SettingDlg();
    userWidget = new UserWidget();
    appWidget = new AppWidget();
    sysWidget = new SystemWidget();
    userDetailWidget = new UserDetailWidget();
    appDetailWidget = new AppDetailWidget();
    stackWidget->insertWidget(0, compuWidget);
    stackWidget->insertWidget(1, userWidget);
    stackWidget->insertWidget(2, appWidget);
    stackWidget->insertWidget(3, sysWidget);
    stackWidget->insertWidget(4, userDetailWidget);
    stackWidget->insertWidget(5, appDetailWidget);
    stackWidget->setContentsMargins(0, 0, 0, 0);
    titleWidget->computer->setCheckable(true);
    titleWidget->computer->setChecked(true);
    mainVlayout = new QVBoxLayout();
    mainVlayout->setMargin(0);
    mainVlayout->setSpacing(0);
    mainVlayout->addWidget(titleWidget);
    mainVlayout->addWidget(stackWidget);
    titleWidget->setContentsMargins(10, 9, 8, 0);
    mainVlayout->setContentsMargins(0, 0, 0, 0);


    networktran = new NetworkTrans();

    infoMsg = new CustomMessageBox ();
    connect(titleWidget, SIGNAL(showMainMenu()), this, SLOT(showMainMenu()));
    connect(titleWidget, SIGNAL(showMin()), this, SLOT(showMinimized()));
    connect(titleWidget, SIGNAL(closeWidget()), this, SLOT(hide()));
    connect(titleWidget, SIGNAL(showLogin()), this, SLOT(networktrans()));

    connect(mainMenu, SIGNAL(showSettingDlg()), this, SLOT(showSettingDialog()));
    connect(mainMenu, SIGNAL(showAbout()), this, SLOT(showAboutUs()));
    connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    connect(systemTray, SIGNAL(showWidget()), this, SLOT(showWidget()));
    connect(titleWidget->computer, SIGNAL(clicked(bool)), this, SLOT(switchComputerWin()));
    connect(titleWidget->user, SIGNAL(clicked(bool)), this, SLOT(switchUserWin()));
    connect(titleWidget->application, SIGNAL(clicked(bool)), this, SLOT(switchAppWin()));
    connect(titleWidget->system, SIGNAL(clicked(bool)), this, SLOT(switchSysWin()));
    connect(userWidget->pictureButton, SIGNAL(clicked(bool)), this, SLOT(switchUserPic()));
    connect(userWidget->musicButton, SIGNAL(clicked(bool)), this, SLOT(switchUserMusic()));
    connect(userWidget->videoButton, SIGNAL(clicked(bool)), this, SLOT(switchUserVideo()));
    connect(userWidget->documentButton, SIGNAL(clicked(bool)), this, SLOT(switchUserDoc()));

    connect(userWidget->pictureButton, SIGNAL(clicked(bool)), userDetailWidget, SLOT(updateDataModel()));
    connect(userWidget->pictureButton, SIGNAL(clicked(bool)), compuWidget, SLOT(progressBarSlot()));
    connect(userWidget->musicButton, SIGNAL(clicked(bool)), userDetailWidget, SLOT(updateDataModel()));
    connect(userWidget->videoButton, SIGNAL(clicked(bool)), userDetailWidget, SLOT(updateDataModel()));
    connect(userWidget->documentButton, SIGNAL(clicked(bool)), userDetailWidget, SLOT(updateDataModel()));
    connect(settingDlg, SIGNAL(modifySetting()), userDetailWidget, SLOT(updateReadIniFilePath()));
    connect(settingDlg, SIGNAL(modifySetting()), userDetailWidget, SLOT(updateDataModel()));
    connect(settingDlg, SIGNAL(modifySetting()), appDetailWidget, SLOT(updateReadIniFilePath()));


    connect(appWidget->emailButton, SIGNAL(clicked(bool)), this, SLOT(switchAppEmail()));
    connect(appWidget->emailButton, SIGNAL(clicked(bool)), appDetailWidget, SLOT(updateEmailDataSlot()));
    connect(appWidget->browseButton, SIGNAL(clicked(bool)), this, SLOT(switchAppBrowser()));
    connect(compuWidget, SIGNAL(exportOnekey()), this, SLOT(onekeyexportSlot()));
    connect(userDetailWidget,SIGNAL(userData_oneKey_progress_num(int,int)),this,SLOT(appData_onekey_progressSlot(int,int)));
    connect(appDetailWidget,SIGNAL(appData_onekey_progress_num(int,int)),this,SLOT(appData_onekey_progressSlot(int,int)));

    connect(infoMsg, SIGNAL(ensureBtnClicked()), this, SLOT(compressData())) ;
    systemTray->show();
    setLayout(mainVlayout);
}

DataMigration::~DataMigration()
{
}

void DataMigration::paintEvent(QPaintEvent *)
{
    QPainter painterBack(this);
    painterBack.drawPixmap(rect(),QPixmap(":/background/back"));

    QPainter painterMain(this);
    QLinearGradient linearMain(rect().topLeft(), rect().bottomLeft());
    linearMain.setColorAt(0, Qt::white);
    linearMain.setColorAt(0.5, Qt::white);
    linearMain.setColorAt(1, Qt::white);
    painterMain.setPen(Qt::white); //设定边框颜色
    painterMain.setBrush(linearMain); //Pen设置的是边框的颜色，Brush设置的方框内部的颜色
    painterMain.drawRect(QRect(0, 109, this->width()-1, this->height()-109-1));

    painterMain.setPen(Qt::lightGray); //设定边框颜色
    painterMain.drawLine(QPoint(0, 109), QPoint(0, this->height()-1));
    painterMain.drawLine(QPoint(0, this->height()-1), QPoint(this->width()-1, this->height()));
    painterMain.drawLine(QPoint(this->width()-1, 109), QPoint(this->width()-1, this->height()));
}

void DataMigration::showMainMenu()
{
    QPoint p = rect().topRight();
    p.setX(p.x() - 100);
    p.setY(p.y() + 32);
    mainMenu->exec(this->mapToGlobal(p));
}


void DataMigration::showSettingDialog()
{
    settingDlg->show();
}

void DataMigration::showAboutUs()
{
    aboutUs->show();
}

void DataMigration::showWidget()
{
    showNormal();
    raise();
    activateWindow();
}

void DataMigration::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
        {
            showWidget();
            break;
        }
        case QSystemTrayIcon::DoubleClick:
        {
            showWidget();
            break;
        }
        default:
            break;
    }
}


void DataMigration::switchComputerWin()
{
    titleWidget->user->setChecked(false);
    titleWidget->user->setCheckable(false);

    titleWidget->application->setChecked(false);
    titleWidget->application->setCheckable(false);

    titleWidget->system->setChecked(false);
    titleWidget->system->setCheckable(false);



    titleWidget->computer->setCheckable(true);
    titleWidget->computer->setChecked(true);
    stackWidget->setCurrentIndex(0);
}

void DataMigration::switchUserWin()
{
    titleWidget->computer->setChecked(false);
    titleWidget->computer->setCheckable(false);
    titleWidget->application->setChecked(false);
    titleWidget->application->setCheckable(false);
    titleWidget->system->setChecked(false);
    titleWidget->system->setCheckable(false);
    titleWidget->user->setCheckable(true);
    titleWidget->user->setChecked(true);
    stackWidget->setCurrentIndex(1);
}

void DataMigration::switchAppWin()
{
    titleWidget->computer->setChecked(false);
    titleWidget->computer->setCheckable(false);
    titleWidget->system->setChecked(false);
    titleWidget->system->setCheckable(false);
    titleWidget->user->setChecked(false);
    titleWidget->user->setCheckable(false);
    titleWidget->application->setCheckable(true);
    titleWidget->application->setChecked(true);
    stackWidget->setCurrentIndex(2);
}

void DataMigration::switchSysWin()
{
    titleWidget->computer->setChecked(false);
    titleWidget->computer->setCheckable(false);
    titleWidget->user->setChecked(false);
    titleWidget->user->setCheckable(false);
    titleWidget->application->setChecked(false);
    titleWidget->application->setCheckable(false);

    titleWidget->system->setCheckable(true);
    titleWidget->system->setChecked(true);
    stackWidget->setCurrentIndex(3);
}


void DataMigration::switchUserPic()
{
    stackWidget->setCurrentIndex(4);
    userDetailWidget->leftListWidget->setCurrentRow(0);
}

void DataMigration::switchUserMusic()
{
    stackWidget->setCurrentIndex(4);
    userDetailWidget->leftListWidget->setCurrentRow(2);
}

void DataMigration::switchUserVideo()
{
    stackWidget->setCurrentIndex(4);
    userDetailWidget->leftListWidget->setCurrentRow(1);
}

void DataMigration::switchUserDoc()
{
    stackWidget->setCurrentIndex(4);
    userDetailWidget->leftListWidget->setCurrentRow(3);
}

void DataMigration::switchAppEmail()
{
    stackWidget->setCurrentIndex(5);
    appDetailWidget->appLeftListWidget->setCurrentRow(0);
}

void DataMigration::switchAppBrowser()
{
    stackWidget->setCurrentIndex(5);
    appDetailWidget->appLeftListWidget->setCurrentRow(1);
}


void DataMigration::exportEmailData()
{
}

void DataMigration::exportBrowerData()
{
}


void DataMigration::onekeyexportSlot()
{
    onekey_over=0;
    sysWidget->system_onekey();
    appDetailWidget->appData_oneKey();
    userDetailWidget->userData_oneKey();
    onekey_count=(appDetailWidget->app_one_count);
    onekey_count+=userDetailWidget->userData_count;
    appDetailWidget->appData_oneKey_start();
    userDetailWidget->userData_oneKey_start();
}

void DataMigration::appData_onekey_progressSlot(int count, int over)
{
    onekey_over+=1;
    compuWidget->setValues(onekey_over/onekey_count*100.0);
    if(onekey_over==onekey_count)
    {
        infoMsg->setInfo(QString("数据迁移"), QString("一键导出完成，是否对数据文件进行压缩？"), QPixmap(":/Message/yes"), true, false);
        infoMsg->exec();
    }
}


void DataMigration::compressData()
{
    QString dstCompressPack = dstFilePath.append(".zip");
    qDebug() << dstCompressPack;
    dstFilePath.remove(".zip");
    JlCompress::compressDir(dstCompressPack, dstFilePath);
}


void DataMigration::networktrans()
{
    networktran->exec();
}
