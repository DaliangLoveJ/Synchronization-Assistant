#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H
#include <QSystemTrayIcon>
class QMenu;
class QAction;

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = 0);
    ~SystemTray();
signals:
    void showWidget();

private:
    QMenu   *trayMenu;
    QAction *actionOpen;
    QAction *actionExit;
public:
    void translateLanguage();
};


#endif // SYSTEMTRAY_H

