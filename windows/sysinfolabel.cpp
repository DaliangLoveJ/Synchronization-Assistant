#include "windows/sysinfolabel.h"
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>

SysInfoLabel::SysInfoLabel(QWidget *parent )
    : QLabel(parent)
{
    this->setWordWrap(true);
    this->setContentsMargins(50, 0, 0, 100);
    this->setStyleSheet("QLabel{color:white; font-size:15px; font-family:微软雅黑;}");
}

SysInfoLabel::~SysInfoLabel()
{
}

void SysInfoLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, labelWidth, labelHeight),pixmap);
    QLabel::paintEvent(event);
}

void SysInfoLabel::loadPixmap(QString picture)
{
    pixmap.load(picture);
    labelWidth = pixmap.width();
    labelHeight = pixmap.height();
    setFixedSize(labelWidth, labelHeight);
}


