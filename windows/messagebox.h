#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H
#include <QDialog>
class QPaintEvent;
class QPushButton;
class QLabel;
class QCheckBox;

class CustomMessageBox : public QDialog
{
    Q_OBJECT
public:
    CustomMessageBox(QWidget *parent = 0);
    ~CustomMessageBox();

    void translateLanguage();
    void setInfo(QString title_info, QString info, QPixmap pixmap, bool is_check_hidden, bool is_ok_hidden);

signals:
    void msgChecked(bool isChecked, bool yes);
    void ensureMessageHidden(bool isChecked);
    void ensureBtnClicked();

private slots:
    void ensureOperate();
    void cancelOperate();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);

public:
    QPoint move_point;
    bool mouse_press ;
    QPushButton *closeButton;
    QPushButton *cancelButton;
    QPushButton *ensueButton;
    QLabel *titleLabel;
    QLabel *msgLabel;
    QLabel *askLabel;
    QCheckBox *checkBox;
    QString ensureText;
    QString cancelText;
};

#endif // MESSAGEBOX_H

