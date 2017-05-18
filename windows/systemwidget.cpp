#include "windows/systemwidget.h"
#include "tools/utils.h"
#include "JlCompress.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QPushButton>

SystemWidget::SystemWidget(QWidget *parent)
    :QWidget(parent)
{

    QVBoxLayout *rightLayout = new QVBoxLayout();
    QHBoxLayout *mainLayout = new QHBoxLayout();

    QHBoxLayout *IPLayout = new QHBoxLayout();
    QHBoxLayout *maskLayout = new QHBoxLayout();
    QHBoxLayout *gatewayLayout = new QHBoxLayout();
    QHBoxLayout *dnsLayout = new QHBoxLayout();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    sysExportBtn = new QPushButton();
    //sysExportBtn
    sysExportBtn->setFixedSize(173, 40);
    sysExportBtn->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/computer/exportone); color:white;}"
                                "QPushButton:hover{border-radius:5px; border-image:url(:/computer/dexportone);}"
                                "QPushButton:pressed{border-radius:5px; border-image:url(:/computer/ddexportone);}");

    btnLayout->addWidget(sysExportBtn);
    btnLayout->setContentsMargins(0, 20, 0, 0);
    QLabel *leftIcon = new QLabel();
    leftIcon->setPixmap(QPixmap(":/syswin/sysnetwork"));
    leftIcon->setFixedWidth(520);

    QLabel *ipNameLabel = new QLabel();
    ipContentLabel = new QLabel();
    ipNameLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    ipContentLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    ipNameLabel->setText(tr("IP"));
    IPLayout->addWidget(ipNameLabel);
    IPLayout->addWidget(ipContentLabel);
    IPLayout->setSpacing(50);

    QLabel *maskNameLabel = new QLabel();
    maskContentLabel = new QLabel();
    maskNameLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    maskContentLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    maskNameLabel->setText(tr("子网掩码"));
    maskLayout->addWidget(maskNameLabel);
    maskLayout->addWidget(maskContentLabel);
    maskLayout->setSpacing(50);

    QLabel *gatewayNameLabel = new QLabel();
    gatewayContentLabel = new QLabel();
    gatewayNameLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    gatewayContentLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    gatewayNameLabel->setText(tr("网关"));
    gatewayLayout->addWidget(gatewayNameLabel);
    gatewayLayout->addWidget(gatewayContentLabel);
    gatewayLayout->setSpacing(50);

    QLabel *dnsNameLabel = new QLabel();
    dnsContentLabel = new QLabel();
    dnsNameLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    dnsContentLabel->setStyleSheet("QLabel{color:gray; font-size:20px; font-family:微软雅黑;}");
    dnsNameLabel->setText(tr("DNS"));
    dnsLayout->addWidget(dnsNameLabel);
    dnsLayout->addWidget(dnsContentLabel);
    dnsLayout->setSpacing(50);

    rightLayout->addLayout(IPLayout);
    rightLayout->addLayout(maskLayout);
    rightLayout->addLayout(gatewayLayout);
    rightLayout->addLayout(dnsLayout);
    rightLayout->setSpacing(15);
    rightLayout->addLayout(btnLayout);
    rightLayout->addStretch();

    //mainLayout->addWidget(leftIcon);
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);
    mainLayout->addSpacing(150);
    mainLayout->setContentsMargins(1, 120, 0, 100);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    _init();

    connect(sysExportBtn, SIGNAL(clicked(bool)), this,SLOT(exportSysSlot()));
    msg=new CustomMessageBox();
    connect(msg, SIGNAL(ensureBtnClicked()), this, SLOT(compressData())) ;
}

SystemWidget::~SystemWidget()
{

}

void SystemWidget::_init()
{
    QList<QNetworkAddressEntry> addlist;
    QString name;
    QList<QNetworkInterface> list=QNetworkInterface::allInterfaces();
    for(int i=0;i<list.count();i++)
    {
        QSettings setting(QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\")+
                          list.at(i).name()+"\\Connection",QSettings::NativeFormat);
        QString value=setting.value("PnpInstanceID").toString();
        if(value.section("\\",0,0).contains("PCI"))
        {
            name=list.at(i).name();
            addlist=list.at(i).addressEntries();
            break;
       }
    }
    QSettings setting(QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\"+name),QSettings::NativeFormat);
    ipContentLabel->setText(setting.value("DhcpIPAddress").toString());
    gatewayContentLabel->setText(setting.value("DhcpServer").toString());
    maskContentLabel->setText(setting.value("DhcpSubnetMask").toString());
    dnsContentLabel->setText(setting.value("DhcpNameServer").toString());
}

void SystemWidget::exportSysSlot()
{
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                          QCoreApplication::organizationName(),
                                          QCoreApplication::applicationName());
    appSetting->beginGroup(DESTINATION_GROUNP);
    QString dstDirPath = appSetting->value(DEFAULT).toString()+QString("\\Network");
    QDir *dir=new QDir();
    dir->setPath(dstDirPath);
    if(dir->exists())
    {
        qDebug()<<"目录已存在";
    }
    else
    {
        dir->mkpath(dstDirPath);
    }
    delete dir;
    QSettings setting(dstDirPath+QString("\\sock.ini"),QSettings::IniFormat);
    setting.beginGroup("Network");
    setting.setValue("IP",ipContentLabel->text());
    setting.setValue("Gate",gatewayContentLabel->text());
    setting.setValue("Mask",maskContentLabel->text());
    setting.setValue("Dns",dnsContentLabel->text());
    setting.endGroup();
    msg->setInfo(QString("数据迁移"), QString("网络数据导出成功，是否对数据文件进行压缩？"), QPixmap(":/Message/yes"), true, false);
    msg->exec();
}

void SystemWidget::system_onekey()
{
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                          QCoreApplication::organizationName(),
                                          QCoreApplication::applicationName());
    appSetting->beginGroup(DESTINATION_GROUNP);
    QString dstDirPath = appSetting->value(DEFAULT).toString()+QString("\\Network");
    QDir *dir=new QDir();
    dir->setPath(dstDirPath);
    if(dir->exists())
    {
        qDebug()<<"目录已存在";
    }
    else
    {
        dir->mkpath(dstDirPath);
    }
    delete dir;
    QSettings setting(dstDirPath+QString("\\sock.ini"),QSettings::IniFormat);
    setting.beginGroup("Network");
    setting.setValue("IP",ipContentLabel->text());
    setting.setValue("Gate",gatewayContentLabel->text());
    setting.setValue("Mask",maskContentLabel->text());
    setting.setValue("Dns",dnsContentLabel->text());
    setting.endGroup();
}

void SystemWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/syswin/sysnetwork"));

    QPainter painterFrame(this);
    painterFrame.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 0), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 0)};
    painterFrame.drawPolyline(points, 4);
}

void SystemWidget::compressData()
{
    QString destination = Util::readDstFileFromIni();
    QString dstCompressPack = destination.append(".zip");
    destination.remove(".zip");
    JlCompress::compressDir(dstCompressPack, destination);
}
