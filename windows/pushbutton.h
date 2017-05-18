#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
class QPainter;
class QMouseEvent;
class QPaintEvent;
class QPixmap;


class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = 0);
    ~PushButton();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseRealeaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    enum ButtonStaus{
        NORMAL,
        ENTER,
        PRESS,
        NOSTATUS
    };
    ButtonStaus status;
    QPixmap pixmap;
    int button_width;
    int button_height;
    bool mouse_ispress;

public:
    void loadPixmap(QString picture);
};
#endif // PUSHBUTTON_H

