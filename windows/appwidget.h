#ifndef APPWIDGET_H
#define APPWIDGET_H
#include <QWidget>
class QPushButton;

class AppWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppWidget(QWidget *parent = 0);
    ~AppWidget();

public:
    QPushButton *browseButton;
    QPushButton *emailButton;
};

#endif // APPWIDGET_H

