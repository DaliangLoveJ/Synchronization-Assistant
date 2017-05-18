#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMenu>
class QAction;

class MainMenu : public QMenu
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();

signals:
    void showSettingDlg();
    void showAbout();

private:
    QAction *actionSetting;
    QAction *actionAbout;

public:
    void translateLanguage();

};
#endif // MAINMENU_H

