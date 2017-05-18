#ifndef USERWIDGET_H
#define USERWIDGET_H
#include "windows/userdetailwidget.h"
#include <QWidget>
class QPushButton;
class QStackedWidget;
class QListWidget;

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = 0);
    ~UserWidget();

public:
    QPushButton *pictureButton;
    QPushButton *musicButton;
    QPushButton *documentButton;
    QPushButton *videoButton;

private:
    QWidget *topWidget;
    QWidget *detailWidget;
    QStackedWidget *topStackWidget;
    QListWidget *detaiLeftWidget;
    QStackedWidget *detailRightWidget;
    UserDetailWidget *userDetailWidget;
    void initUserTopWidget();


};
#endif // USERWIDGET_H

