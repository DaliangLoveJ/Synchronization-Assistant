#ifndef NETWORKTRANS_H
#define NETWORKTRANS_H
#include "windows/messagebox.h"
#include "tools/ftpmanager.h"
#include <QDialog>
#include <QAbstractSocket>

class QMouseEvent;
class QLabel;
class QPushButton;
class QLineEdit;
class QProgressBar;
class QTcpSocket;
class QFile;
class QNetworkReply;

class NetworkTrans : public QDialog
{
    Q_OBJECT
public:
    explicit NetworkTrans(QWidget *parent = 0);
    ~NetworkTrans();

private:
    /*gui*/
    QLabel *titleLabel;
    QLabel *titleIcon;
    QPushButton *closeButton;
    QPushButton *ensureButton;
    QPoint move_point;
    bool   mouse_press;

    QLabel *userName;
    QLabel *passwd;
    QLabel *netPort;
    QLabel *netPath;
    QLabel *netIP;

    QLabel *filePath;
    QLineEdit *fileInput;
    QLineEdit *usernameEdit;
    QLineEdit *passwdEdit;
    QLineEdit *portEdit;
    QLineEdit *pathEdit;
    QLineEdit *ipEdit;

    QPushButton *browser;
    QPushButton *sendFile;
    QPushButton *quitCur;

    QProgressBar *transProg;
    CustomMessageBox *msgInfo;
    FtpManager m_ftp;
    void checkCompressFile();
    void checkElement();
    void loadSetting();
    void saveSettings();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

private slots:
    void on_openButton_clicked();
    void on_sendButton_clicked();
    void quitButton_clicked();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError );
};


#endif // NETWORKTRANS_H

