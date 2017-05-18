#ifndef MAINTITLE
#define MAINTITLE

#include "pushbutton.h"
#include "toolbutton.h"
#include <QWidget>

class QLabel;
class QPushButton;
class QPoint;
class QHBoxLayout;
class QLabel;

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
    ~TitleWidget();

signals:
    void showSkin();
    void showMin();
    void showMax();
    void showMainMenu();
    void closeWidget();
    void showLogin();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    bool       move_is;
    QPoint     press_point;
    QPushButton *login_button;
    QPushButton *setup_button;
    QPushButton *min_button;
    QPushButton *max_button;
    QPushButton *close_button;
    QLabel     *title_text;
    QLabel     *title_icon;
    QHBoxLayout *title_layout;
    QLabel      *logo;
    QLabel      *dataText;

public:
    ToolButton  *computer;
    ToolButton  *user;
    ToolButton  *application;
    ToolButton  *system;
    void initWidget();
    void translateLanguage();
};

#endif // MAINTITLE

