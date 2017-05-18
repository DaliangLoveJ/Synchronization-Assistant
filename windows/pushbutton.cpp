#include "windows/pushbutton.h"
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>


PushButton::PushButton(QWidget *parent )
    : QPushButton(parent)
{
    status = NORMAL;
    mouse_ispress = false;
}

PushButton::~PushButton()
{
}


void PushButton::enterEvent(QEvent *)
{
    status = ENTER;
    update();
}
void PushButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_ispress = true;
        status = PRESS;
        update();
        emit clicked();
    }
}

void PushButton::mouseRealeaseEvent(QMouseEvent *)
{
    if(mouse_ispress)
    {
        mouse_ispress = false;
        status = ENTER;
        update();
    }
}

void PushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), pixmap.copy(button_width * status, 0, button_width, button_height));
}


void PushButton::loadPixmap(QString picture)
{
    pixmap.load(picture);
    button_width = pixmap.width();
    button_height = pixmap.height();
    setFixedSize(button_width,button_height);
}
