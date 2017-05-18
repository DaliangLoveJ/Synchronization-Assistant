#ifndef OUTLOOKDATA
#define OUTLOOKDATA
/*该文件依赖windows平台*/
#include <QDir>
#include <QThread>
#include <QDebug>
class QString;
class QStringList;


class EmailData :public QThread
{
    Q_OBJECT
public:
    EmailData();
    ~EmailData();

    QStringList outlookScanPath;
    QList <QString> outlookotherScanPath;
    QString outlookToPath;
    QString foxmailToPath;
    QStringList getOutlookPath();
    QList <QString> findOutlookPst();
    bool copyPstFileTo(QString dstDir,QList<QString> other);
    QStringList foxmailScanPath;
    bool copyEmlFileTo(QString dstDir);
    bool checkOutlook();
    bool checkFoxmail();
    QStringList getFoxmailAccount();
    QStringList getOutlookAccount();
    QString  getFoxmailExe();
    bool autoFoxmailAuto();
    virtual void run(void);
    int emailcount;
signals:
    void emailData_error_num(int out,int fox);
    void emailData_progress_num(double num);
    void emailData_one_progress_num(int coun,int over);
};



#endif // OUTLOOKDATA

