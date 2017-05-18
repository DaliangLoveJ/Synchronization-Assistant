#include "windows/computerwidget.h"
#include "windows/QRoundProgressBar.h"
#include "tools/utils.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColor>
#include <QTimer>

ComputerWidget::ComputerWidget(QWidget *parent)
    :QWidget(parent)
{
    compuLabel = new SysInfoLabel();
    progressBar = new QRoundProgressBar();
    sysverLabel = new QLabel();
    onebtnExport = new QPushButton();
    QGradientStops gradientPoints;
    gradientPoints << QGradientStop(0, QColor(149, 210, 244)) << QGradientStop(0.5, QColor(51, 153, 255)) << QGradientStop(1, QColor(31, 136, 216));

    QPalette pal;
    pal.setBrush(QPalette::Base, QColor(240, 240, 240));
    pal.setColor(QPalette::Background, Qt::white);
    pal.setColor(QPalette::Text, Qt::blue);
    pal.setColor(QPalette::Shadow, QColor(149, 210, 244));
    progressBar->setPalette(pal);
    progressBar->setNullPosition(QRoundProgressBar::PositionRight);
    progressBar->setBarStyle(QRoundProgressBar::StylePie);
    progressBar->setDataColors(gradientPoints);
    progressBar->setFixedSize(225, 225);
    progressBar->setValue(0);
    //progressBar
    compuLabel->loadPixmap(":/computer/maincom");
    onebtnExport->setFixedSize(173, 40);
    onebtnExport->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/computer/exportone); color:white;}"
                                "QPushButton:hover{border-radius:5px; border-image:url(:/computer/dexportone);}"
                                "QPushButton:pressed{border-radius:5px; border-image:url(:/computer/ddexportone);}");
    sysverLabel->setStyleSheet("QLabel{color:lightgray; font-size:24px; font-family:微软雅黑;}");
//    sysverLabel->setText(tr("Windows7 专业版"));
    compuLabel->setStyleSheet("QLabel{color:white; font-size:24px; font-family:微软雅黑;}");
    _init();
//    compuLabel->setText(tr("Windows7 专业版"));
    compuLabel->setContentsMargins(70, 10, 0, 75);

    QHBoxLayout *mainHLayout = new QHBoxLayout();
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();
    leftLayout->addWidget(compuLabel, 0, Qt::AlignCenter);
    //leftLayout->addWidget(sysverLabel, 0, Qt::AlignCenter);
    leftLayout->setContentsMargins(0, 50, 0, 50);

    rightLayout->addWidget(progressBar, 0, Qt::AlignCenter);
    rightLayout->addWidget(onebtnExport, 0, Qt::AlignCenter);
    rightLayout->setSpacing(10);
    rightLayout->setContentsMargins(0, 80, 0, 60);

    mainHLayout->addLayout(leftLayout);
    mainHLayout->addLayout(rightLayout);
    mainHLayout->setContentsMargins(0, 0, 0, 30);
    setLayout(mainHLayout);
    connect(onebtnExport, SIGNAL(clicked(bool)), this, SIGNAL(exportOnekey()));
}

ComputerWidget::~ComputerWidget()
{
}

void ComputerWidget::setValues(double value)
{
    progressBar->setValue(value);
}

void ComputerWidget::progressBarSlot()
{
}

void ComputerWidget::_init()
{
    QSettings setting(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),QSettings::NativeFormat);
    QString sv=setting.value("EditionID").toString();
    if(sv=="Ultimate")
    {
        sv=tr("旗舰版");
    }
    else if(sv=="Core")
    {
        sv=tr("核心版");
    }
    else if(sv=="Home Premium")
    {
        sv=tr("高级家庭版");
    }
    else if(sv=="Server")
    {
        sv=tr("服务器版");
    }
    else if(sv=="Home Basic")
    {
        sv=tr("基础家庭版");
    }
    QString sn=setting.value("ProductName").toString();
    compuLabel->setText(sn.section(" ",0,0)+" "+sn.section(" ",1,1)+" "+sv);
}
