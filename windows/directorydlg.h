#ifndef DIRECTORYDLG_H
#define DIRECTORYDLG_H
#include <QDialog>
#include <QPoint>
#include <QModelIndex>
class QListView;
class QStandardItemModel;

class DirectoryDlg : public QDialog
{
    Q_OBJECT
public:
    explicit DirectoryDlg(QString userGroup, QWidget *parent = 0);
    ~DirectoryDlg();
    void readIniFile();
    bool createPathListView();

signals:
    void iniFileModify();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void addPathSlot();
    void removePathSlot();
    void cancelBtnSlot();

private:
    QPoint move_point;
    bool   mouse_press;
    QStringList allPath;
    QListView *leftPathView;
    QModelIndex currentIndex;
    QStandardItemModel *listItemModel;
    QString   user_group;
    QPushButton *cancelBtn;
    QPushButton *applyBtn;

};

#endif // DIRECTORYDLG_H

