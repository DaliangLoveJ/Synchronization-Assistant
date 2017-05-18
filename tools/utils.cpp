#include "functions/common.h"
#include "utils.h"
#include <QString>
#include <windows.h>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QSettings>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QSysInfo>

#define REG_SYSTEM_DIR "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"


bool Util::copyFileTo(QString src_file, QString dst_path, bool coverFileIfExist)
{
    if(src_file == dst_path)
        return true;
    if (!QFile::exists(src_file))
        return false;

    QDir *targetfile =  new QDir;
    QFileInfo *info = new QFileInfo(dst_path);
    bool file_exist = targetfile->exists(dst_path);
    if (file_exist)
    {
        if(coverFileIfExist)
        {
            targetfile->remove(dst_path);
        }
    }
    else
    {
        if(!(targetfile->exists(info->path())))
        {
            targetfile->mkpath(info->path());
        }
    }
    if(!QFile::copy(src_file, dst_path))
    {
        qDebug()<<"Copy Failed...";
        return false;
    }
    return true;
}

 bool Util::copyDirTo(const QString &src_dir, const QString &dst_dir, bool coverFileIfExist)
 {
    QDir source(src_dir);
    QDir target(dst_dir);
    if(!target.exists())  /*如果目标不存在，就进行创建*/
    {
       if(!target.mkdir(target.absolutePath()))  /*如果目标路径创建不成功，返回false*/
           return false;
    }
    QFileInfoList srcFileInfoList = source.entryInfoList();
    foreach (QFileInfo fileInfo, srcFileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir()) /*子目录的情况，递归进行复制*/
        {
            if( !copyDirTo(fileInfo.filePath(), target.filePath(fileInfo.fileName()), coverFileIfExist))
                return false;
        }
        else
        {
            if(coverFileIfExist && target.exists(fileInfo.fileName()))
                target.remove(fileInfo.fileName());

            if( !QFile::copy(fileInfo.filePath(), target.filePath(fileInfo.fileName())))
                return false;
        }
    }
    return true;
 }


 QMap<QString, QString> Util::findFiles(QString filename, QString path)
 {
     QDir currentDir;
     QMap<QString, QString> FilePath;
     currentDir = QDir(path);
     QStringList files;
     QString     file_path;
//     if (filename.isEmpty())
//         filename = "*.pst";

     files = currentDir.entryList(QStringList(filename), QDir::Files | QDir::NoSymLinks);
     if(!files.isEmpty())
     {
         for(int i = 0; i < files.size(); ++i)
         {
             file_path = currentDir.absoluteFilePath(files[i]);
             FilePath.insert(files[i], file_path);
         }
         return FilePath;
     }
 }


QList< QString > Util::findFileList(QString filename, QString path)
{
    QDir scanDir(path);
    QString filesPath;
    QStringList filesName;
    QList < QString > totalFilesList;
    filesName = scanDir.entryList(QStringList(filename), QDir::Files | QDir::NoSymLinks);
    if(!filesName.isEmpty())
    {
        for (int i=0; i < filesName.size(); ++i)
        {
            filesPath = scanDir.absoluteFilePath(filesName[i]);
            totalFilesList.append(filesPath);
        }
    }
    return totalFilesList;
}

 QMap<QString, QString> Util::findDirs(QString dirName, QString path)
 {
     QMap<QString, QString> dirPath;
     QDir    searchDir;
     QString file_path;
     QStringList dirList;
     QStringList resultList;

     searchDir = QDir(path);
     dirList = searchDir.entryList(QDir::Dirs, QDir::Name);
     resultList = dirList.filter(dirName);
     if (!resultList.isEmpty())
     {
         for(int i = 0; i < resultList.size(); ++i)
         {
           file_path = searchDir.absoluteFilePath(resultList[i]);
           dirPath.insert(resultList[i],file_path);
          }
         return dirPath;
     }
 }

/*QString path参数传递时*/
QList< QString > Util::findDirList(QString dirname, QString path)
{
    QList< QString >  dirPathList;
    QString     dirPath;
    QStringList dirInList;
    QStringList resultInList;
    QDir searchDir = QDir(path);
    dirInList = searchDir.entryList(QDir::Dirs, QDir::Name);
    resultInList = dirInList.filter(dirname);
    if(!resultInList.isEmpty())
    {
        for(int i=0; i < resultInList.size(); ++i )
        {
            dirPath = searchDir.absoluteFilePath(resultInList[i]);
            dirPathList.append(dirPath);
        }
    }
    else
    {
       qDebug() << "Could not found this dir...";
    }
    return dirPathList;
}

bool Util::setOrganAppName(QString organizationName, QString applicationName)
{
 QCoreApplication::setOrganizationName(organizationName);
 QCoreApplication::setApplicationName(applicationName);
}



bool Util::writeInitFile(QString userGroup, QString userKey, QString userValue)
{
     QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
     if(userGroup.isEmpty() || userKey.isEmpty() || userKey.isEmpty())
     {
         delete appSetting;
         return false;
     }
     else
     {
         appSetting->beginGroup(userGroup);
         appSetting->setValue(userKey, userValue);
         appSetting->endGroup();
         delete appSetting;
         return true;
     }
}

 bool Util::readInitFile( QString userGroup, QString userKey, QString &userValue)
 {
      QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

      userValue = QString("");
      if(userGroup.isEmpty() || userKey.isEmpty() )
      {
          return false;
          delete appSetting;
      }
      else
      {
          appSetting->beginGroup(userGroup);
          userValue = appSetting->value(userKey).toString();
          appSetting->endGroup();
          delete appSetting;
          return true;
      }
 }


QString Util::getSysFavouriteDir()
{
    QString sysFavouriteDir;
    QSettings *regDir = new QSettings(REG_SYSTEM_DIR, QSettings::NativeFormat);
    sysFavouriteDir = regDir->value("Favorites", false).toString();
    if (sysFavouriteDir.isEmpty())
    {
         qDebug() << "System Favourite Dir is not exist...";
    }
    delete regDir;
    return sysFavouriteDir;
}

QString Util::getSysMusicDir()
{
    QString syMusicDir;
    QSettings *regDir = new QSettings(REG_SYSTEM_DIR, QSettings::NativeFormat);
    syMusicDir = regDir->value("My Music", false).toString();
    if (syMusicDir.isEmpty())
    {
         qDebug() << "System Music Dir is not exist...";
    }
    delete regDir;
    return syMusicDir;
}

QString Util::getSysPictureDir()
{
    QString syPictureDir;
    QSettings *regDir = new QSettings(REG_SYSTEM_DIR, QSettings::NativeFormat);
    syPictureDir = regDir->value("My Pictures", false).toString();
    if (syPictureDir.isEmpty())
    {
         qDebug() << "System Pictures Dir is not exist...";
    }
    delete regDir;
    return syPictureDir;
}

QString Util::getSysVideoDir()
{
    QString syVideoDir;
    QSettings *regDir = new QSettings(REG_SYSTEM_DIR, QSettings::NativeFormat);
    syVideoDir = regDir->value("My Video", false).toString();
    if (syVideoDir.isEmpty())
    {
         qDebug() << "System Video Dir is not exist...";
    }
    delete regDir;
    return syVideoDir;
}


QString Util::getSysAppDataDir()
{
    QString syAppDataDir;
    QSettings *regDir = new QSettings(REG_SYSTEM_DIR, QSettings::NativeFormat);
    syAppDataDir = regDir->value("AppData", false).toString();
    if (syAppDataDir.isEmpty())
    {
         qDebug() << "System AppData Dir is not exist...";
    }
    delete regDir;
    return syAppDataDir;
}



QString Util::getLocalAppDataDir()
{
    QString syLocalAppDataDir;
    QSettings *regDir = new QSettings(REG_SYSTEM_DIR, QSettings::NativeFormat);
    syLocalAppDataDir = regDir->value("Local AppData", false).toString();
    if (syLocalAppDataDir.isEmpty())
    {
         qDebug() << "System Local AppData Dir is not exist...";
    }
    delete regDir;
    return syLocalAppDataDir;
}

QString Util::getUserDocDir()
{
    QString syUserDocDir;
    QSettings *regDir = new QSettings(REG_SYSTEM_DIR, QSettings::NativeFormat);
    syUserDocDir = regDir->value("Personal", false).toString();
    if (syUserDocDir.isEmpty())
    {
         qDebug() << "System Document Dir is exist...";
    }
    delete regDir;
    return syUserDocDir;
}


QString Util::readDstFileFromIni()
{
    QString dstPath;
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->beginGroup(DESTINATION_GROUNP);
    dstPath = appSetting->value(DEFAULT).toString();
    appSetting->endGroup();
    delete appSetting;
    return dstPath;
}


void  Util::getSysType()
{
//    char *ch;
//    ch = getenv("PROCESSOR_LEVEL");
//    QString osType;
//    osType.sprintf("%s", ch);
//    qDebug() << osType;
//    return osType;

    /*
SYSTEM_INFO systemInfo;
GetSystemInfo(&systemInfo);

qDebug() << QStringLiteral("处理器掩码：%1").arg(systemInfo.dwActiveProcessorMask);
qDebug() << QStringLiteral("处理器个数：%1").arg(systemInfo.dwNumberOfProcessors);
qDebug() << QStringLiteral("处理器分页大小：%1").arg(systemInfo.dwPageSize);



*/
   SYSTEM_INFO systemInfo;
   GetSystemInfo(&systemInfo);
   qDebug() << QStringLiteral("处理器掩码：%1").arg(systemInfo.dwActiveProcessorMask);
   qDebug() << QStringLiteral("处理器个数：%1").arg(systemInfo.dwNumberOfProcessors);
   qDebug() << QStringLiteral("处理器分页大小：%1").arg(systemInfo.dwPageSize);
   qDebug() << QStringLiteral("处理器类型：%1").arg(systemInfo.dwProcessorType);
   qDebug() << QStringLiteral("最大寻址单元：") << systemInfo.lpMaximumApplicationAddress;
   qDebug() << QStringLiteral("最小寻址单元：") << systemInfo.lpMinimumApplicationAddress;
   qDebug() << QStringLiteral("处理器等级：%1").arg(systemInfo.wProcessorLevel);
   qDebug() << QStringLiteral("处理器版本：%1").arg(systemInfo.wProcessorRevision);

   QSysInfo::WinVersion win = QSysInfo::windowsVersion();
   qDebug() << win;
}

QFileInfoList Util::convenienceDir(QString dirPath)
 {
     QFileInfoList relist;
     QDir *dir=new QDir();
     dir->setPath(dirPath);
     QFileInfoList infolist=dir->entryInfoList();
     for(int i=0;i<infolist.count();i++)
     {
         QFileInfo info=infolist.at(i);
         if(info.fileName()=="."||info.fileName()=="..")
         {
             continue;
         }
         if(info.isDir())
         {
             QFileInfoList inl=convenienceDir(info.filePath());
             for(int j=0;j<inl.count();j++)
             {
                 relist.append(inl.at(j));
             }
         }
         else
         {
             relist.append(info);
         }
     }
     delete dir;
     return relist;
 }
