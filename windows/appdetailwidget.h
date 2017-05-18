#ifndef APPDETAILWIDGET_H
#define APPDETAILWIDGET_H
#include <QDialog>
class QWidget;
class QLabel;
class QListWidget;
class QStackedWidget;
class QPoint;
class QLineEdit;
class QComboBox;
class QPushButton;
class BrowserData;
class EmailData;
class QTreeView;
class QStandardItemModel;
class QStandardItem;
class CustomDelegate;
class QProgressBar;
class CustomMessageBox;
class QModelIndex;
#define APPDATA_EMAIL 4
#define APPDATA_BROWSER 5
class AppDetailWidget : public QDialog
{
    Q_OBJECT
public:
    explicit AppDetailWidget(QWidget *parent = 0);
    ~AppDetailWidget();

protected:
    void paintEvent(QPaintEvent *);
    void reject();

public slots:
    void updateBrowserSlot();
    void updateEmailDataSlot();
    void emailBtnSlot(const QModelIndex &index);
    void updateReadIniFilePath();

private slots:
    void treeItemChanged(QStandardItem * item);
    void treeItem_checkAllChild(QStandardItem * item,bool check = true);
    void treeItem_checkAllChild_recursion(QStandardItem * item,bool check = true);
    Qt::CheckState checkSibling(QStandardItem * item);
    void treeItem_CheckChildChanged(QStandardItem * item);
    void iecomboboxdelete(const QModelIndex &index);
    void firefoxcomboboxdelete(const QModelIndex &index);
    void chromecomboboxdelete(const QModelIndex &index);

    void IEBrowserAdd();
    void FirefoxBrowserAdd();
    void ChromeBrowserAdd();
    void IECheckBoxSlot(int state);
    void ChromeCheckBoxSlot(int state);
    void FireFoxCheckBoxSlot(int state);
    void exportBrowserDataSlot();
    void exportEmailDataSlot();
    void getEmailTreeView(const QModelIndex &index);
    void getemailItemData(QStandardItem * item);

    void emailData_error_num_slot(int out,int fox);
    void emailData_current_num_slot(double num);

    void browserData_error_name_slot(int ie,int ch,int fir);
    void browserData_progress_num_slot(double num);

    void browserData_one_progress_num_slot(int coun,int over);
    void emailData_one_progress_num_slot(int coun,int over);
    void compressData();


signals:
    void appData_onekey_progress_num(int coun,int over);

public:
    QLabel *browserInfo;
    QLabel *emailInfo;
    BrowserData *browser;
    EmailData *email;
    QString IEDir;
    QList <QString> chromeFile;
    QList <QString> firefoxFile;
    QListWidget *appLeftListWidget;
    QStackedWidget *appRightStackWidget;
    QWidget *emailDetalWidget;
    QWidget *browserDetailWidget;
    QProgressBar *emailProgressBar;
    QProgressBar *browserProgressBar;
    CustomMessageBox *msg;

    void translateLanguage();
    void initAppEmailWidget();
    void initAppBrowerWidget();
    void updateBrowserData();
    void updateEmailData();
    void checkClient();

    void appData_oneKey(void);
    void appData_oneKey_start(void);
    BrowserData *browser_one;
    EmailData *email_one;
    int app_one_count;

private:
    bool updateFoxmail();
    QComboBox *outlookFileCombox;
    QComboBox *foxmailFileCombox;
    QComboBox  *IEcombox;
    QComboBox  *firfoxCombox;
    QComboBox  *chromeCombox;
    QPushButton *outlookBtn;
    QPushButton *foxmailBtn;
    QPushButton *ieBtn;
    QPushButton *firfoxBtn;
    QPushButton *chromeBtn;
    QPushButton *emailExportToPath;
    QPushButton *browserExportToPath;

    QList < QString > IEFileList;
    QList < QString > chromeFileList;
    QList < QString > firfoxFileList;
    QString dstFilePath;
    QTreeView          *emailTreeView;
    QStandardItemModel *emailModel;
    CustomDelegate     *customDelegate;
    QStandardItem      *outlookItem;
    QStandardItem      *foxmailItem;

    QStringList IECopyFileList;
    QStringList ChromeCopyFileList;
    QStringList FireFoxCopyFileList;

    QStringList outlookFileList;
    QStringList foxmailFileList;
    QStringList emailFileList;

    int app_one_email_over;
    int app_one_browser_over;


};

#endif // APPDETAILWIDGET_H

