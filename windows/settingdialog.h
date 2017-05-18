#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H
#include "windows/directorydlg.h"
#include <QDialog>
class QWidget;
class QLabel;
class QListWidget;
class QStackedWidget;
class QPoint;
class QLineEdit;
class QComboBox;


class SettingDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDlg(QWidget *parent = 0);
    ~SettingDlg();
signals:
    void modifySetting();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

public slots:
    void switchPicSetting();
    void switchMusSetting();
    void switchDocSetting();
    void switchVidSetting();
    void setupDstDir();
    void readUpdateDisplay();
    void restoreIniFile();
private:
    QLabel *titleLabel;
    QLabel *titleIcon;
    QPushButton *closeButton;
    QPushButton *ensureButton;
    QListWidget *leftWidget;
    QStackedWidget *rightWidget;
    QPoint move_point;
    bool   mouse_press;
    QWidget *srcWidget;
    QWidget *toWidget;

    QComboBox *picLineEdit;
    QComboBox *musicLineEdit;
    QComboBox *videoLineEdit;
    QComboBox *docLineEdit;
    QPushButton *picBtn;
    QPushButton *musicBtn;
    QPushButton *videoBtn;
    QPushButton *docBtn;
    QPushButton *restoreBtn;

    QComboBox *toDirLineEdit;
    QPushButton *toDirBtn;
    QPushButton *restoreToBtn;
    QPushButton *cancelBtn;
public:
    DirectoryDlg *picDlg;
    DirectoryDlg *musDlg;
    DirectoryDlg *vidDlg;
    DirectoryDlg *docDlg;
    void translateLanguage();
    void initSrcWidget();
    void initToWidget();
    void readInitDisplay();
};



#endif // SETTINGDIALOG_H

