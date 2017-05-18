#ifndef USERDETAILWIDGET_H
#define USERDETAILWIDGET_H
#include "functions/common.h"
#include <QDialog>
#include "functions/userdata.h"
#include <QThread>
class QWidget;
class QLabel;
class QListWidget;
class QStackedWidget;
class QPoint;
class QLineEdit;
class TableModel;
class QTableView;
class CheckBoxDelegate;
class QCheckBox;
class QProgressBar;
class CustomMessageBox;

class UserDetailWidget : public QDialog
{
    Q_OBJECT
public:
    explicit UserDetailWidget(QWidget *parent = 0);
    ~UserDetailWidget();
    void translateLanguage();
    void userData_oneKey(void);
    void userData_oneKey_start(void);

protected:
    void paintEvent(QPaintEvent *);
    void reject();

private:
    qint64 picFileSize = 0;
    qint64 vidFileSize = 0;
    qint64 musFileSize = 0;
    qint64 docFileSize = 0;
    QStringList picCopyFileList;
    QStringList vidCopyFileList;
    QStringList musCopyFileList;
    QStringList docCopyFileList;
    int picCopyFileCount = 0;
    int vidCopyFileCount = 0;
    int musCopyFileCount = 0;
    int docCopyFileCount = 0;

    QString dstDirPath;
    QStringList picScanPath;
    QStringList musScanPath;
    QStringList vidScanPath;
    QStringList docScanPath;
    TableModel *picModel;
    TableModel *vidModel;
    TableModel *musModel;
    TableModel *docModel;
    QList < QString > picFileList;
    QList < QString > musFileList;
    QList < QString > vidFileList;
    QList < QString > docFileList;    
    QTableView *picTableView;
    QTableView *vidTableView;
    QTableView *musTableView;
    QTableView *docTableView;
    CheckBoxDelegate *picDelegate;
    CheckBoxDelegate *vidDelegate;
    CheckBoxDelegate *musDelegate;
    CheckBoxDelegate *docDelegate;
    QPushButton *picExportToPath;
    QPushButton *vidExportToPath;
    QPushButton *musExportToPath;
    QPushButton *docExportToPath;

    void initPicWidget();
    void initVidWidget();
    void initMusWidget();
    void initDocWidget();
    void getInitFilePath();
    void setDataAllModel();

    QCheckBox *picAllchecked;
    QCheckBox *musAllchecked;
    QCheckBox *docAllchecked;
    QCheckBox *vidAllchecked;

    QProgressBar *picProgressBar;
    QProgressBar *musProgressBar;
    QProgressBar *vidProgressBar;
    QProgressBar *docProgressBar;
    CustomMessageBox *msg_pic;
    CustomMessageBox *msg_mus;
    CustomMessageBox *msg_vid;
    CustomMessageBox *msg_doc;
    QThread *th_doc;
    QThread *th_mus;
    QThread *th_pic;
    QThread *th_vid;
    QThread *th_one;
    UserData *userdata_doc;
    UserData *userdata_mus;
    UserData *userdata_pic;
    UserData *userdata_vid;
    UserData *userdata_one;


    int userData_over;

    void updataTable(int c,QString name,QString stat);

public slots:
    void updateReadIniFilePath();
    void updateDataModel();
    void getPicCheckedSlot(const QModelIndex &index);
    void getVidCheckedSlot(const QModelIndex &index);
    void getMusicCheckedSlot(const QModelIndex &index);
    void getDocCheckedSlot(const QModelIndex &index);
    void getPicChecked(const QModelIndex &index);
    void getVidChecked(const QModelIndex &index);
    void getMusicChecked(const QModelIndex &index);
    void getDocChecked(const QModelIndex &index);
    void picCheckedBoxAll(int state);
    void vidCheckedBoxAll(int state);
    void musCheckedBoxAll(int state);
    void docCheckedBoxAll(int state);
    void exportPicSlot();
    void exportVidSlot();
    void exportMusSlot();
    void exportDocSlot();

    void UserData_error_name_Slot(int c,QString name);
    void UserData_num_Slot(int c,double num);
    void UserData_over_name_slot(int c,QString name);
    void UserData_copy_num_slot(int c,int num);
    void UserData_current_copy_name_slot(int c,QString name);

    void UserData_one_overSlot(int c,int name);
    void UserData_progress_num_slot(int c,int cout,int over);
    void compressData();

signals:
    void userData_oneKey_over(int c,int name);
    void userData_oneKey_progress_num(int cou,int over);

public:
    QListWidget *leftListWidget;
    QStackedWidget *rightStackWidget;
    QWidget *picDetalWidget;
    QWidget *vidDetailWidget;
    QWidget *musDetailWidget;
    QWidget *docDetailWidget;
    int userData_count;
};


#endif // USERDETAILWIDGET_H

