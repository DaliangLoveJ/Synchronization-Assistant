#include "windows/progresslabel.h"
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>

ProgressLabel::ProgressLabel(QWidget *parent )
    : QLabel(parent)
{
    this->setContentsMargins(130, 90, 0, 100);
    this->setStyleSheet("QLabel{color:white; font-size:25px; font-family:微软雅黑;}");
}

ProgressLabel::~ProgressLabel()
{
}

void ProgressLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, labelWidth, labelHeight),pixmap);
    QLabel::paintEvent(event);
}

void ProgressLabel::loadPixmap(QString picture)
{
    pixmap.load(picture);
    labelWidth = pixmap.width();
    labelHeight = pixmap.height();
    setFixedSize(labelWidth, labelHeight);
}



