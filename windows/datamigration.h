#ifndef DATAMIGRATION_H
#define DATAMIGRATION_H

#include "windows/titlewidget.h"
#include "windows/mainmenu.h"
#include "windows/systemtray.h"
#include "windows/computerwidget.h"
#include "windows/aboutus.h"
#include "windows/settingdialog.h"
#include "windows/userwidget.h"
#include "windows/appwidget.h"
#include "windows/systemwidget.h"
#include "windows/userdetailwidget.h"
#include "windows/appdetailwidget.h"

#include <QWidget>
class QVBoxLayout;
class QStackedWidget;
class CustomMessageBox;
class NetworkTrans;

class DataMigration : public QWidget
{
    Q_OBJECT

public:
    DataMigration(QWidget *parent = 0);
    ~DataMigration();

public slots:
    void showWidget();

private slots:
    void showMainMenu();
    void showSettingDialog();
    void showAboutUs();
    void iconIsActived(QSystemTrayIcon::ActivationReason reason);
    void switchComputerWin();
    void switchUserWin();
    void switchAppWin();
    void switchSysWin();
    void switchUserPic();
    void switchUserMusic();
    void switchUserVideo();
    void switchUserDoc();
    void switchAppEmail();
    void switchAppBrowser();
    void exportEmailData();
    void exportBrowerData();
    void onekeyexportSlot();
    void compressData();
    void appData_onekey_progressSlot(int count,int over);
    void networktrans();

protected:
    void paintEvent(QPaintEvent *);

private:
    TitleWidget *titleWidget;
    QVBoxLayout *mainVlayout;
    MainMenu    *mainMenu;
    SystemTray  *systemTray;
    QStackedWidget *stackWidget;
    ComputerWidget *compuWidget;
    AboutUsDialog  *aboutUs;
    SettingDlg     *settingDlg;
    UserWidget     *userWidget;
    AppWidget      *appWidget;
    SystemWidget   *sysWidget;
    UserDetailWidget *userDetailWidget;
    AppDetailWidget  *appDetailWidget;
    CustomMessageBox *infoMsg;
    NetworkTrans     *networktran;

    int onekey_count;
    double onekey_over;
    QString dstFilePath;
};

#endif // DATAMIGRATION_H
