#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H
#include <QToolButton>
class QMouseEvent;
class QString;
class QPaintEvent;

class ToolButton : public QToolButton
{
  Q_OBJECT
public:
  ToolButton(QString picture, QWidget *parent = 0);
  ~ToolButton();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

public:
    void setMousePress(bool mouse_press);
    bool mouse_isover;
    bool mouse_ispress;
};

#endif // TOOLBUTTON_H

