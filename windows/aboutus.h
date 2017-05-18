#ifndef ABOUTUS_H
#define ABOUTUS_H
#include <QDialog>
class QLabel;
class QPoint;
class QPushButton;

class AboutUsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutUsDialog(QWidget *parent = 0);
    ~AboutUsDialog();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

private:
    QPoint move_point;
    bool   mouse_press;
    QLabel *titleLabel;
    QLabel *titleIcon;
    QLabel *titleInfo;
    QLabel *infoLabel;
    QLabel *versionLabel;
    QLabel *authorLabel;
    QLabel *copyrightLabel;
    QLabel *iconLabel;
    QPushButton *closeButton;
    QPushButton *ensureButton;

public:
    void translateLanguage();
};

#endif // ABOUTUS_H

