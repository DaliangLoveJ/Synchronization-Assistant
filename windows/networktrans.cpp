#include "windows/networktrans.h"
#include "tools/utils.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QLineEdit>
#include <QProgressBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QTcpSocket>
#include <QSettings>


NetworkTrans::NetworkTrans(QWidget *parent) :
      QDialog(parent)
{
    resize(460, 200);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowIcon(QPixmap(":/title/logo"));
    titleIcon = new QLabel();
    titleLabel = new QLabel();
    closeButton = new QPushButton();
    closeButton->setFixedSize(18, 18);
    closeButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/close);}"
                               "QPushButton:hover{border-radius:5px; border-image:url(:/title/dclose);}"
                               "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dclose);}");
    titleLabel->setStyleSheet("QLabel{color:white;}");
    titleLabel->setText(tr("网络传输数据"));
    titleIcon->setPixmap(QPixmap(":/title/logo"));
    titleIcon->setFixedSize(16, 16);
    titleIcon->setScaledContents(true);
    closeButton->setContentsMargins(0, 0, 0, 5);


    userName = new QLabel();
    passwd = new QLabel();
    netPort = new QLabel();
    netPath = new QLabel();
    netIP = new QLabel();
    filePath = new QLabel();

    userName ->setText(tr("用户名"));
    passwd->setText(tr("密码"));
    netPort->setText(tr("端口"));
    netPath->setText(tr("路径"));
    netIP->setText(tr("地址"));
    filePath->setText(tr("文件"));

    filePath->setStyleSheet("QLabel{color:rgb(43,165,237); font: bold; font-size:12px; font-family:黑体;}");
    netIP->setStyleSheet("QLabel{color:rgb(43,165,237); font: bold; font-size:12px; font-family:黑体;}");
    userName->setStyleSheet("QLabel{color:rgb(43,165,237); font: bold; font-size:12px; font-family:黑体;}");
    passwd->setStyleSheet("QLabel{color:rgb(43,165,237); font: bold; font-size:12px; font-family:黑体;}");
    netPort->setStyleSheet("QLabel{color:rgb(43,165,237); font: bold; font-size:12px; font-family:黑体;}");
    netPath->setStyleSheet("QLabel{color:rgb(43,165,237); font: bold; font-size:12px; font-family:黑体;}");
    usernameEdit = new QLineEdit();
    passwdEdit = new QLineEdit();
    portEdit = new QLineEdit();
    pathEdit = new QLineEdit();
    ipEdit = new QLineEdit();
    fileInput = new QLineEdit();

    ipEdit->setInputMask("000.000.000.000; _");
    ipEdit->setStyleSheet("QLineEdit{font-size:12px; font-family:黑体;}");

    browser = new QPushButton();
    browser->setText(tr("..."));
    sendFile = new QPushButton();
    sendFile->setText(tr("上传"));
    quitCur = new QPushButton();
    quitCur->setText(tr("退出"));

    browser->setFixedSize(40, 20);
    browser->setStyleSheet("QPushButton{border:1px solid lightgray; background:rgb(230,230,230);}"
                                   "QPushButton:hover{border-color:green; background:transparent;}");
    sendFile->setFixedSize(86, 26);
    sendFile->setStyleSheet("QPushButton{border:1px solid lightgray; background:rgb(230,230,230);}"
                               "QPushButton:hover{border-color:green; background:transparent;}");
    quitCur->setFixedSize(86, 26);
    quitCur->setStyleSheet("QPushButton{border:1px solid lightgray; background:rgb(230,230,230);}"
                               "QPushButton:hover{border-color:green; background:transparent;}");

    transProg = new QProgressBar();
    transProg->setStyleSheet("QProgressBar{border:none; color:white; text-align:center; background:white; }"
                  "QProgressBar::chunk{background: rgb(0, 160, 230); }");


    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIcon, 0, Qt::AlignVCenter);
    titleLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton, 0, Qt::AlignVCenter);
    titleLayout->setSpacing(5);
    titleLayout->setContentsMargins(4, 3, 4, 0);


    usernameEdit->setFixedWidth(150);
    QHBoxLayout *userLayout = new QHBoxLayout();
    userLayout->addWidget(userName);
    userLayout->addWidget(usernameEdit);
    userLayout->addSpacing(12);
    userLayout->addWidget(passwd);
    userLayout->addWidget(passwdEdit);
    userLayout->setContentsMargins(15, 0, 15, 0);

    ipEdit->setFixedWidth(150);
    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(netIP);
    portLayout->addSpacing(12);
    portLayout->addWidget(ipEdit);
    portLayout->addSpacing(12);
    portLayout->addWidget(netPort);
    portLayout->addWidget(portEdit);
    portLayout->setContentsMargins(15, 0, 15, 0);

    QHBoxLayout *fileInputLayout = new QHBoxLayout();
    fileInputLayout->addWidget(filePath);
    fileInputLayout->addSpacing(12);
    fileInputLayout->addWidget(fileInput);
    fileInputLayout->addWidget(browser);
    fileInputLayout->setContentsMargins(15, 0, 15, 0);

    QHBoxLayout *buttonTrig = new QHBoxLayout();
    buttonTrig->addSpacing(270);
    buttonTrig->setSpacing(8);
    buttonTrig->addWidget(sendFile);
    buttonTrig->addWidget(quitCur);
    buttonTrig->setContentsMargins(0, 0, 12, 0);

    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->addWidget(transProg);
    progressLayout->setContentsMargins(30, 0, 30, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(userLayout);
    mainLayout->addLayout(portLayout);
    mainLayout->addLayout(fileInputLayout);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(buttonTrig);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(progressLayout);
    mainLayout->addStretch();
    mainLayout->setMargin(2);
    setLayout(mainLayout);
    checkCompressFile();

    msgInfo = new CustomMessageBox(this);
    connect(browser, SIGNAL(clicked()), this, SLOT(on_openButton_clicked()));
    connect(sendFile, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
    connect(quitCur, SIGNAL(clicked()), this, SLOT(quitButton_clicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));

    loadSetting();

}

NetworkTrans::~NetworkTrans()
{
    saveSettings();
}

void NetworkTrans::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        mouse_press = true;
    }
    move_point = event->globalPos() - pos();
}

void NetworkTrans::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}

void NetworkTrans::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_press)
    {
        QPoint move_pos = event->globalPos();
        move(move_pos - move_point);
    }
}

void NetworkTrans::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/background/back"));
    QPainter painterWidget(this);
    QLinearGradient linearWidget(rect().topLeft(), rect().bottomLeft());
    linearWidget.setColorAt(0, Qt::white);
    linearWidget.setColorAt(0.5, Qt::white);
    linearWidget.setColorAt(1, Qt::white);
    painterWidget.setPen(Qt::white);
    painterWidget.setBrush(linearWidget);
    painterWidget.drawRect(QRect(0, 30, this->width(), this->height()-30));


    QPainter painterFrame(this);
    painterFrame.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 30), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 30)};
    painterFrame.drawPolyline(points, 4);
}


void NetworkTrans::checkCompressFile()
{
    QString  fileCompress = Util::readDstFileFromIni();
    //fileCompress.remove("\\");
    QString  addFile = fileCompress.append(".zip");
    qDebug()<< addFile;
    fileCompress.remove(".zip");

    QFileInfo fileCheck(addFile);
    if(fileCheck.isFile())
    {
        //fileInput->setText(addFile);
    }
}

void NetworkTrans::on_openButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("添加上传文件"), NULL, tr("文件 (*.* )"));
    if (fileName.isEmpty())
    {
        return;
    }
    fileInput->setText(fileName);
}

void NetworkTrans::on_sendButton_clicked()
{

    if(fileInput->text().isEmpty() | usernameEdit->text().isEmpty() | passwdEdit->text().isEmpty()
            | portEdit->text().isEmpty() | ipEdit->text().isEmpty())
    {

        msgInfo->setInfo(QString("网络传输"), QString("请填写网络传输的基本要素"), QPixmap(":/Message/question"), true, true);
        msgInfo->exec();
    }
    else
    { 

        m_ftp.setHostPort(ipEdit->text(), portEdit->text().toInt());
        m_ftp.setUserInfo(usernameEdit->text(), passwdEdit->text());
        m_ftp.put(fileInput->text(), "/DataMigration.zip");
        connect(&m_ftp, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        connect(&m_ftp, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64, qint64)));
    }
}

void NetworkTrans::quitButton_clicked()
{
    close();
}

void NetworkTrans::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    transProg->setMaximum(bytesTotal);
    transProg->setValue(bytesSent);
}

void NetworkTrans::error(QNetworkReply::NetworkError error)
{
    switch (error)
    {
        case QNetworkReply::HostNotFoundError :
            qDebug() << QString::fromLocal8Bit("主机没有找到");
            break;
        default:
            break;
    }
}

void NetworkTrans::checkElement()
{
    if(fileInput->text().isEmpty() | usernameEdit->text().isEmpty() | passwdEdit->text().isEmpty()
            | portEdit->text().isEmpty() | ipEdit->text().isEmpty())
    {
        msgInfo->setInfo(QString("网络传输"), QString("请填写网络传输的基本要素"), QPixmap(":/Message/question"), true, true);
        msgInfo->exec();
    }
}

void NetworkTrans::loadSetting()
{
    QString defaultPort;
    defaultPort = "2121";

    QSettings settings;
    portEdit->setText(settings.value("settings/port", defaultPort).toString());
    usernameEdit->setText(settings.value("settings/username", "dataMigration").toString());
    passwdEdit->setText(settings.value("settings/password", "nfschina").toString());
}

void NetworkTrans::saveSettings()
{
    QSettings settings;
    settings.setValue("settings/port", portEdit->text());
    settings.setValue("settings/username", usernameEdit->text());
    settings.setValue("settings/password", passwdEdit->text());
}
