#include "windows/toolbutton.h"
#include <QMouseEvent>
#include <QToolButton>
#include <QString>
#include <QPaintEvent>
#include <QPalette>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QMargins>

ToolButton::ToolButton(QString picture, QWidget *parent)
    : QToolButton(parent)
{
    /*set text color*/
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::ButtonText,QColor(230,230,230));
    setPalette(text_palette);

    /*set bold font*/
    QFont &text_font = const_cast<QFont &>(font());
    text_font.setWeight(QFont::Bold);
    text_font.setFamily("微软雅黑");
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    /*set the button icon*/
    QPixmap pixmap(picture);
    setIcon(pixmap);
    setIconSize(pixmap.size());
    setFixedSize(pixmap.width()+35, pixmap.height()+35);
    setAutoRaise(true);
    setContentsMargins(QMargins(0, 0, 0, 20));
    setStyleSheet("background:transparent");

    mouse_isover = false;
    mouse_ispress = false;
}

ToolButton::~ToolButton()
{
}


void ToolButton::enterEvent(QEvent *)
{
    mouse_isover = true;
}

void ToolButton::leaveEvent(QEvent *)
{
     mouse_isover = false;
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

void ToolButton::paintEvent(QPaintEvent *event)
{
    /*QPainter painter(this);
    QPen pen(Qt::NoBrush, 1);
    painter.setPen(pen);
    QLinearGradient linear(rect().topLeft(), rect().bottomLeft());

    linear.setColorAt(0, QColor(230, 230, 150, 5));
    linear.setColorAt(0.5, QColor(30, 100, 200, 100));
    linear.setColorAt(1, QColor(30, 100, 200, 150));
    painter.setBrush(linear);

    if(mouse_isover)
    {
        painter.drawRect(rect());
    }
    else
    {
        if(mouse_ispress)
        {
            painter.drawRect(rect());
        }
    }*/

    QToolButton::paintEvent(event);
}



void ToolButton::setMousePress(bool mouse_press)
{
    mouse_ispress = mouse_press;
    update();
}
