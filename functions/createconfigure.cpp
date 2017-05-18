#include "functions/createconfigure.h"
#include "tools/utils.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>


ConfigureDirTool::ConfigureDirTool(QString organName, QString appName )
{
    this->organization = organName;
    this->application = appName;
}

ConfigureDirTool::~ConfigureDirTool()
{

}

void ConfigureDirTool::createIniFile()
{
    Util::setOrganAppName(organization, application);
    Util::writeInitFile(DOCUMENT_GROUNP , Util::getUserDocDir(), "DEFAULT");
    Util::writeInitFile(PICTURE_GROUNP , Util::getSysPictureDir(), "DEFAULT");
    Util::writeInitFile(VIDEO_GROUNP , Util::getSysVideoDir(), "DEFAULT");
    Util::writeInitFile(MUSIC_GROUNP , Util::getSysMusicDir(), "DEFAULT");
    Util::writeInitFile(DESTINATION_GROUNP, Util::readDstFileFromIni(), "DEFAULT");
}

bool ConfigureDirTool::makeDstDir()
{
    QString readDstDir;
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    appSetting->beginGroup(DESTINATION_GROUNP);
    readDstDir = appSetting->value("DEFAULT").toString();
    appSetting->endGroup();
    delete appSetting;

    readDstDir.append("\\");
    bool   mkDstDirFlag;
    QDir *dstDir = new QDir();
    bool dstExist = dstDir->exists(readDstDir);
    if (dstExist)
    {
        return true;
    }
    else
    {
        mkDstDirFlag = dstDir->mkdir(readDstDir);
        if (mkDstDirFlag)
            return true;
        else
            return false;
    }
}


