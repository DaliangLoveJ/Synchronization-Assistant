#ifndef UTIL_H
#define UTIL_H

#include <QMap>
#include <windows.h>
#include <QFileInfoList>
class QString;

class Util
{
public:
    static bool copyFileTo(QString src_file, QString dst_path, bool coverFileIfExist);
    static bool copyDirTo(const QString &src_dir, const QString &dst_dir, bool coverFileIfExist);
    static QMap<QString, QString> findFiles(QString filename, QString path);
    static QMap<QString, QString> findDirs(QString dirname, QString path);
    static QList< QString > findFileList(QString filename, QString path);
    static QList< QString > findDirList(QString dirname, QString path);
    static bool setOrganAppName(QString organizationName, QString applicationName);
    static bool readInitFile( QString userGroup, QString userKey, QString &userValue);
    static bool writeInitFile(QString userGroup, QString userKey, QString userValue);
    static QString getSysFavouriteDir();
    static QString getSysMusicDir();
    static QString getSysPictureDir();
    static QString getSysAppDataDir();
    static QString getSysVideoDir();
    static QString getLocalAppDataDir();
    static QString getUserDocDir();
    static QString readDstFileFromIni();
    static void getSysType();

    static QFileInfoList convenienceDir(QString dirPath);


};

#endif // UTIL_H
