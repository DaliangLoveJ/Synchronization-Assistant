#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>
#include <QString>
#include "tools/utils.h"
#include <QFileInfo>
#define USERDATA_DOC 3
#define USERDATA_MUS 2
#define USERDATA_VID 1
#define USERDATA_PIC 0
#define USERDATA_ONE 4
class UserData : public QObject
{
    Q_OBJECT
public:
    explicit UserData(QObject *parent = 0);
    QStringList CopyList;
//    QString DstPath;
    QMap<QString,QString> CopyToDst;
    int current_index;
signals:
    void UserData_error_name(int c,QString name);
    void UserData_over_name(int c,QString name);
    void UserData_copy_num(int c,int num);
    void UserData_current_copy_name(int c,QString name);
    void UserData_progress_num(int c,int count,int over);
public slots:
    void UserDataCopyToPath(void);
};

#endif // USERDATA_H
