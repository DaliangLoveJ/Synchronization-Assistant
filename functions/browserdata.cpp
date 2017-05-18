#include "functions/browserdata.h"
#include "tools/utils.h"
#include <QString>
#include <QStringList>
#include <QDesktopServices>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

BrowserData::BrowserData()
{
    IEDir = scanIEFavorites();
    chromeFile = scanChromeFavorites();
    firefoxFile = scanFirefoxFavorites();
}

BrowserData::~BrowserData()
{
}

QString BrowserData::scanIEFavorites()
{
    QString IEPath = QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    IEPath.append("/Favorites/");
    QFileInfo IEInfo(IEPath);
    if (IEInfo.isDir())
    {
        return IEPath;
    }
    else
    {
        QString outPut = QString("%1 is not exist...").arg(IEPath);
        qDebug() << outPut;
    }
}

QList <QString> BrowserData::scanChromeFavorites()
{
    QList <QString> chromeFavorites;
    QString chromeFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    chromeFilePath.append("/Google/Chrome/User Data/Default/");
    QFileInfo chromeDirInfo(chromeFilePath);
    if(chromeDirInfo.isDir())
    {
       chromeFavorites = Util::findFileList("Bookmarks", chromeFilePath);
       if(chromeFavorites.isEmpty())
       {
           qDebug() << "Chrome Browser Favorites is not exist...";
       }
    }
    else
    {
        qDebug() << "Chrome Browser is not install...";
    }
    return chromeFavorites;
}

QList <QString> BrowserData::scanFirefoxFavorites()
{
    QList <QString> firefoxFavorites;
    QList <QString> firefoxTmp;
    QString firefoxDetailPath;

    QString firefoxPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    firefoxPath.append("/AppData/Roaming/Mozilla/Firefox/Profiles/");
    QFileInfo firefoxInfo(firefoxPath);
    if(firefoxInfo.isDir())
    {
        firefoxTmp = Util::findDirList(".default", firefoxPath);
        if(!firefoxTmp.isEmpty())
        {
            foreach(firefoxDetailPath, firefoxTmp)
            {
                firefoxFavorites = Util::findFileList("places.sqlite",firefoxDetailPath);
            }
        }
        else
        {
            qDebug() << "Firefox Browser is not installed / Favorites is not exist...";
        }
    }
    else
    {
        qDebug() << "Firefox Browser is not installed ...";
    }
    return firefoxFavorites;
}


void BrowserData::exportBrowserData()
{
    browsercount=IEcopylist.count()+Chromecopylist.count()+Firefoxcopylist.count();
    int c=0;
    int ie=0,ch=0,fir=0;
    if (!IEcopylist.isEmpty())
    {
        int i=0;
        QString IEFile;
        foreach(IEFile, IEcopylist)
        {
            QString DstPath=IEDstPathlist.at(i);
            QFileInfo IEFileInfo(IEFile);
            QString IEDstFileName = IEFileInfo.fileName();
            DstPath.append(IEDstFileName);
            if(!Util::copyFileTo(IEFile, DstPath, true))
            {
                ie++;
            }
            DstPath.remove(IEDstFileName);
            c++;
            i++;
            emit BrowserData_progress_num(c/browsercount*100);
            emit BrowserData_one_progress_num(browsercount,c);
        }
    }
    if(!Chromecopylist.isEmpty())
    {
        QString IEFile;
        QString DstPath=ChromeDstPath;
        foreach(IEFile, Chromecopylist)
        {
            QFileInfo IEFileInfo(IEFile);
            QString IEDstFileName = IEFileInfo.fileName();
            DstPath.append(IEDstFileName);
            if(!Util::copyFileTo(IEFile, DstPath, true))
            {
                ch++;
            }
            DstPath.remove(IEDstFileName);
            c++;
            emit BrowserData_progress_num(c/browsercount*100);
            emit BrowserData_one_progress_num(browsercount,c);
        }
    }
    if(!Firefoxcopylist.isEmpty())
    {
        QString IEFile;
        QString DstPath=FirefoxDstPath;
        foreach(IEFile, Firefoxcopylist)
        {
            QFileInfo IEFileInfo(IEFile);
            QString IEDstFileName = IEFileInfo.fileName();
            DstPath.append(IEDstFileName);
            if(!Util::copyFileTo(IEFile, DstPath, true))
            {
                fir++;
            }
            DstPath.remove(IEDstFileName);
            c++;
            emit BrowserData_progress_num(c/browsercount*100);
            emit BrowserData_one_progress_num(browsercount,c);
        }
    }
    emit BrowserData_progress_num(100);
    emit BrowserData_one_progress_num(browsercount,browsercount);
    emit BrowserData_error_name(ie,ch,fir);
}


bool BrowserData::checkIEBowser()
{

}

bool BrowserData::checkFirefoxBrowser()
{

}

bool BrowserData::checkChromeBrowser()
{

}

void BrowserData::run()
{
    exportBrowserData();
}
