#ifndef SYSINFOLABEL_H
#define SYSINFOLABEL_H

#include <QLabel>
class QPainter;
class QMouseEvent;
class QPaintEvent;
class QPixmap;
class QString;


class SysInfoLabel : public QLabel
{
  Q_OBJECT
public:
    explicit SysInfoLabel(QWidget *parent = 0);
    ~SysInfoLabel();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap pixmap;
    int labelWidth;
    int labelHeight;
public:
    void loadPixmap(QString picture);
};
#endif
