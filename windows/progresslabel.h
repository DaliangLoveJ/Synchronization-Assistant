#ifndef PROGRESSLABEL
#define PROGRESSLABEL
#include <QLabel>
class QPainter;
class QMouseEvent;
class QPaintEvent;
class QPixmap;
class QString;


class ProgressLabel : public QLabel
{
  Q_OBJECT
public:
    explicit ProgressLabel(QWidget *parent = 0);
    ~ProgressLabel();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap pixmap;
    int labelWidth;
    int labelHeight;
public:
    void loadPixmap(QString picture);
};

#endif // PROGRESSLABEL

