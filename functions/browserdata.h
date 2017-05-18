#ifndef BROWSER_H
#define BROWSER_H
/*该文件依赖windows平台*/

#include <QString>
#include <QMap>
#include <QList>
#include <QThread>

class BrowserData :public QThread
{
    Q_OBJECT
public:
    BrowserData();
    ~BrowserData();
    QString IEDir;
    QList <QString> chromeFile;
    QList <QString> firefoxFile;
    bool checkIEBowser();
    bool checkFirefoxBrowser();
    bool checkChromeBrowser();
    QString scanIEFavorites();
    QList <QString> scanChromeFavorites();
    QList <QString> scanFirefoxFavorites();
    void exportBrowserData();
    QStringList IEcopylist;
    QStringList Chromecopylist;
    QStringList Firefoxcopylist;
    QStringList IEDstPathlist;
    QString ChromeDstPath;
    QString FirefoxDstPath;
    int browsercount;
    virtual void run(void);
signals:
    void BrowserData_error_name(int ie,int ch,int fir);
    void BrowserData_progress_num(double num);
    void BrowserData_one_progress_num(int count,int over);
};

#endif // BROWSER_H
