#include "functions/emaildata.h"
#include "tools/utils.h"
#include <QString>
#include <QStringList>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <windows.h>

#define REG_EMAIL_EXISTS  "HKEY_LOCAL_MACHINE\\SOFTWARE\\Clients\\Mail"
#define REG_FOXMAIL_PATH  "HKEY_CURRENT_USER\\Software\\Aerofox\\FoxmailPreview"


EmailData::EmailData()
{
   outlookScanPath = getOutlookPath();
}

EmailData::~EmailData()
{
}

QStringList EmailData::getOutlookPath()
{
    /*-----C:\Users\Thinking\AppData\Local\Microsoft\Outlook---- */
    QStringList outlookPath;
    QString outlookGeneric = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    outlookGeneric.append("/Microsoft/Outlook/");
    QFileInfo outlookGenericInfo(outlookGeneric);
    if(outlookGenericInfo.isDir())
    {
        outlookPath.append(outlookGeneric);
    }
    /*----C:\Users\Thinking\Documents\Outlook 文件----*/
    QString outlookChDoc = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    outlookChDoc.append("/Outlook 文件/");
    QFileInfo outlookChInfo(outlookChDoc);
    if (outlookChInfo.isDir())
    {
        outlookPath.append(outlookChDoc);
    }
    /*----C:\Users\Thinking\AppData\Roaming\Microsoft\Outlook----*/
    QString outlookRoamTime = QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    outlookRoamTime.append("/AppData/Roaming/Microsoft/Outlook/");
    QFileInfo outlookRoamTimeInfo(outlookRoamTime);
    if(outlookRoamTimeInfo.isDir())
    {
        outlookPath.append(outlookRoamTime);
    }
    /*"C:/Users/Thinking/Documents/Outlook Files/"*/
    QString outlookEnDoc = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    outlookEnDoc.append("/Outlook Files/");
    QFileInfo outlookEnDocInfo(outlookEnDoc);
    if (outlookEnDocInfo.isDir())
    {
       outlookPath.append(outlookEnDoc);
    }
    /*"C:/Users/Thinking/Roaming/Local/Microsoft/Outlook/"*/
    QString outlookRunTime = QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    outlookRunTime.append("/Roaming/Local/Microsoft/Outlook/");
    QFileInfo outlookRunTimeInfo(outlookRunTime);
    if(outlookRunTimeInfo.isDir())
    {
       outlookPath.append(outlookRunTime);
    }
    return outlookPath;
}

QList <QString> EmailData::findOutlookPst()
{
    QList <QString> allOutlookPst;
    QList <QString> OutlookPst;
    QString everyPath;
    QStringList outl=getOutlookPath();
    foreach(everyPath, outl)
    {
        OutlookPst = Util::findFileList("*.pst", everyPath);
        allOutlookPst.append(OutlookPst);
    }
    return allOutlookPst;
}

/*QString dstDir参数传递时，一定要在最后加上双斜杠*/



bool EmailData::checkOutlook()
{
    QStringList mailResult;
    QSettings *emailReg = new QSettings(REG_EMAIL_EXISTS, QSettings::NativeFormat);
    mailResult = emailReg->childGroups();
    delete emailReg;
    if (mailResult.contains("Microsoft Outlook"))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool EmailData::checkFoxmail()
{
    QStringList mailResult;
    QSettings *emailReg = new QSettings(REG_EMAIL_EXISTS, QSettings::NativeFormat);
    mailResult = emailReg->childGroups();
    delete emailReg;
    if (mailResult.contains("Foxmail"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QStringList EmailData::getFoxmailAccount()
{
    QStringList accountList;
    QString storgePath;
    QStringList accountFileList;
    QSettings *foxmailReg = new QSettings(REG_FOXMAIL_PATH, QSettings::NativeFormat);
    if(checkFoxmail())
    {
        storgePath = foxmailReg->value("Executable", false).toString();
        storgePath.remove("Foxmail.exe");
        storgePath.append("Storage");
        QDir accountFile(storgePath);
        accountFileList = accountFile.entryList(QDir::Dirs);
        accountFileList.removeFirst();
        accountFileList.removeFirst();
    }
    if (!accountFileList.isEmpty())
    {
        accountList = accountFileList;
    }
    delete foxmailReg;
    return accountList;
}

QStringList EmailData::getOutlookAccount()
{
    QStringList accountOutlook;
    QStringList accountOutlookR;
    QList <QString> outlookPst;
    outlookPst = findOutlookPst();
    if (!outlookPst.isEmpty())
    {
        QString outlookFile;
        foreach (outlookFile, outlookPst) {
             QFileInfo outlookInfo(outlookFile);
             accountOutlook.append(outlookInfo.fileName());
        }
    }
    if(!accountOutlook.isEmpty())
    {
//        if(accountOutlook.contains("Outlook.pst"))
//        {
//            accountOutlook.removeOne("Outlook.pst");
//        }
        QString accountName;
        foreach (accountName, accountOutlook)
        {
//            accountName.remove(".pst");
            accountOutlookR.append(accountName);
        }
    }
    return accountOutlookR;
}

QString  EmailData::getFoxmailExe()
{
    QString exePath;
    QSettings *foxmailReg = new QSettings(REG_FOXMAIL_PATH, QSettings::NativeFormat);
    if(checkFoxmail())
    {
       exePath = foxmailReg->value("Executable", false).toString();
    }
    delete foxmailReg;
    return exePath;
}

bool EmailData::autoFoxmailAuto()
{
    /*邮件数据存储位置 C:\Users\Thinking\Documents\Datamigration\Foxmail\ */
    QString foxmailExePath = getFoxmailExe();
    if (foxmailExePath.isEmpty())
    {
        return false;
    }
    if (checkFoxmail())
    {
        /*调用外部foxmail进程*/
    }
    else
    {
        /*未安装Foxmail*/
    }
}

bool EmailData::copyPstFileTo(QString dstDir,QList<QString> other)
{
    bool ret=true;
    QList <QString> pstFile;
    QString pstDetail;
    pstFile = outlookScanPath;
    if (pstFile.isEmpty()&&other.isEmpty())
    {
        return false;
    }
    else
    {
        double i=0;
        if(!pstFile.isEmpty())
        {
            foreach(pstDetail, pstFile)
            {
                QFileInfo pstInfo(pstDetail);
                QString pstName = pstInfo.fileName();
                dstDir.append(pstName);
                if(!Util::copyFileTo(pstDetail, dstDir, true))
                {
                    ret=false;
                }
                dstDir.remove(pstName);
                i++;
                emit emailData_progress_num(i/emailcount*100);
                emit emailData_one_progress_num(emailcount,i);
            }
        }
        if(!other.isEmpty())
        {
            foreach(pstDetail, other)
            {
                QFileInfo pstInfo(pstDetail);
                QString pstName = pstInfo.fileName();
                dstDir.append(pstName);
                if(!Util::copyFileTo(pstDetail, dstDir, true))
                {
                    ret=false;
                }
                dstDir.remove(pstName);
                i++;
                emit emailData_progress_num(i/emailcount*100);
                emit emailData_one_progress_num(emailcount,i);
            }
        }
    }
    return ret;
}

/*注意dstDir参数传递*/
bool EmailData::copyEmlFileTo(QString dstDir)
{
    bool ret=true;
    double i=outlookScanPath.count()+outlookotherScanPath.count();
    if(!foxmailScanPath.isEmpty())
    {
        QString pstDetail;
        foreach(pstDetail, foxmailScanPath)
        {
            QFileInfo pstInfo(pstDetail);
            QString pstName = pstInfo.fileName();
            dstDir.append(pstName);
            if(!Util::copyFileTo(pstDetail, dstDir, true))
            {
                ret=false;
            }
            dstDir.remove(pstName);
            i++;
            emit emailData_progress_num(i/emailcount*100);
            emit emailData_one_progress_num(emailcount,i);
        }
        return ret;
    }
    else
    {
        return false;
    }
}

void EmailData::run()
{
    bool p,e;
    emailcount=0;
    emailcount=outlookScanPath.count()+outlookotherScanPath.count()+foxmailScanPath.count();
    emit emailData_progress_num(0);
    p=copyPstFileTo(outlookToPath,outlookotherScanPath);
    e=copyEmlFileTo(foxmailToPath);
    outlookToPath.clear();
    outlookotherScanPath.clear();
    foxmailToPath.clear();
    emit emailData_progress_num(100);
    emit emailData_one_progress_num(emailcount,emailcount);
    emit emailData_error_num(p,e);
}























