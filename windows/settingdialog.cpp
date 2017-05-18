#include "windows/settingdialog.h"
#include "functions/common.h"
#include "functions/createconfigure.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QListWidget>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QFileDialog>


SettingDlg::SettingDlg(QWidget *parent)
    :QDialog(parent)
{
    resize(520, 280);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowIcon(QPixmap(":/title/logo"));

    picDlg = new DirectoryDlg(PICTURE_GROUNP);
    musDlg = new DirectoryDlg(MUSIC_GROUNP);
    vidDlg = new DirectoryDlg(VIDEO_GROUNP);
    docDlg = new DirectoryDlg(DOCUMENT_GROUNP);


    titleIcon = new QLabel();
    titleLabel = new QLabel();
    closeButton = new QPushButton();
    closeButton->setFixedSize(18, 18);
    closeButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/close);}"
                               "QPushButton:hover{border-radius:5px; border-image:url(:/title/dclose);}"
                               "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dclose);}");
    titleLabel->setStyleSheet("QLabel{color:white;}");
    titleIcon->setPixmap(QPixmap(":/title/logo"));
    titleIcon->setFixedSize(16, 16);
    titleIcon->setScaledContents(true);
    closeButton->setContentsMargins(0, 0, 0, 5);
    leftWidget = new QListWidget();
    rightWidget = new QStackedWidget();
    QListWidgetItem *srcDir = new QListWidgetItem(tr("源路径设置"));
    QListWidgetItem *toDir = new QListWidgetItem(tr("目标路径设置"));
    srcDir->setTextAlignment(Qt::AlignCenter);
    toDir->setTextAlignment(Qt::AlignCenter);
    leftWidget->setFocusPolicy(Qt::NoFocus);
    leftWidget->setFixedWidth(100);
    leftWidget->insertItem(0,srcDir);
    leftWidget->insertItem(1,toDir);;
    leftWidget->setContentsMargins(0, 0, 0, 0);
    leftWidget->setStyleSheet("QListWidget{border:0px;background-color:rgb(244, 244, 244)}"
                              "QListWidget::item{color:rgb(101, 101, 101); height: 32px;}"
                              "QListWidget::item:hover{color:white; background-color:rgb(51, 168, 235);}"
                              "QListWidget::item:selected{border:0px; color:white; background-color:rgb(51, 168, 235);}");

    leftWidget->setCurrentRow(0);
    initSrcWidget();
    initToWidget();
    rightWidget->insertWidget(0, srcWidget);
    rightWidget->insertWidget(1, toWidget);
    cancelBtn = new QPushButton();
    cancelBtn->setFixedSize(90, 23);
    cancelBtn->setStyleSheet("QPushButton{border:1px solid lightgray; background:rgb(230,230,230);}"
                             "QPushButton:hover{border-color:green; background:transparent;}");

    QHBoxLayout *bottomHlayout = new QHBoxLayout();
    bottomHlayout->addSpacing(450);
    bottomHlayout->addWidget(cancelBtn);
    bottomHlayout->setContentsMargins(0, 0, 20, 0);
    QHBoxLayout *centerHlayout = new QHBoxLayout();
    centerHlayout->addWidget(leftWidget, 0, Qt::AlignLeft);
    centerHlayout->addWidget(rightWidget);
    centerHlayout->setContentsMargins(0, 12, 0, 0);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIcon, 0, Qt::AlignVCenter);
    titleLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton, 0, Qt::AlignVCenter);
    titleLayout->setSpacing(5);
    titleLayout->setContentsMargins(4, 0, 4, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(centerHlayout);
    mainLayout->addLayout(bottomHlayout);
    mainLayout->addSpacing(8);
    mainLayout->setContentsMargins(1, 6, 0, 1);
    setLayout(mainLayout);
    connect(leftWidget, SIGNAL(currentRowChanged(int)), rightWidget,SLOT(setCurrentIndex(int)));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    translateLanguage();
    readInitDisplay();    
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(hide()));
    connect(picDlg, SIGNAL(iniFileModify()), this, SLOT(readUpdateDisplay()));
    connect(musDlg, SIGNAL(iniFileModify()), this, SLOT(readUpdateDisplay()));
    connect(vidDlg, SIGNAL(iniFileModify()), this, SLOT(readUpdateDisplay()));
    connect(docDlg, SIGNAL(iniFileModify()), this, SLOT(readUpdateDisplay()));   

    connect(picDlg, SIGNAL(iniFileModify()), this, SIGNAL(modifySetting()));
    connect(musDlg, SIGNAL(iniFileModify()), this, SIGNAL(modifySetting()));
    connect(vidDlg, SIGNAL(iniFileModify()), this, SIGNAL(modifySetting()));
    connect(docDlg, SIGNAL(iniFileModify()), this, SIGNAL(modifySetting()));
}

SettingDlg::~SettingDlg()
{
}


void SettingDlg::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        mouse_press = true;
    }
    move_point = event->globalPos() - pos();
}

void SettingDlg::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}

void SettingDlg::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_press)
    {
        QPoint move_pos = event->globalPos();
        move(move_pos - move_point);
    }
}

void SettingDlg::paintEvent(QPaintEvent *)
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

    QPainter painterLeft(this);
    QLinearGradient linearLeft(rect().topLeft(), rect().bottomLeft());
    linearLeft.setColorAt(0, QColor(244,244,244));
    linearLeft.setColorAt(0.5, QColor(244,244,244));
    linearLeft.setColorAt(1, QColor(244,244,244));
    painterLeft.setPen(QColor(244,244,244));
    painterLeft.setBrush(linearLeft);
    painterLeft.drawRect(QRect(0, 30, 100, this->height()-30-1));

    QPainter painterFrame(this);
    painterFrame.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 30), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 30)};
    painterFrame.drawPolyline(points, 4);
}

void SettingDlg::translateLanguage()
{
    titleLabel->setText(tr("数据迁移-设置"));
    cancelBtn->setText(tr("返回"));
}

void SettingDlg::initSrcWidget()
{
    srcWidget = new QWidget();
    QLabel *picLabel = new QLabel();
    QLabel *musicLabel = new QLabel();
    QLabel *videoLabel = new QLabel();
    QLabel *docLabel = new QLabel();
    picLabel->setText(tr("图片"));
    musicLabel->setText(tr("音乐"));
    videoLabel->setText(tr("视频"));
    docLabel->setText(tr("文档"));

    picLabel->setStyleSheet("QLabel{color:gray;font-size:14px; font-family:微软雅黑;}");
    musicLabel->setStyleSheet("QLabel{color:gray;font-size:14px; font-family:微软雅黑;}");
    videoLabel->setStyleSheet("QLabel{color:gray;font-size:14px; font-family:微软雅黑;}");
    docLabel->setStyleSheet("QLabel{color:gray;font-size:14px; font-family:微软雅黑;}");

    picLineEdit = new QComboBox();
    musicLineEdit = new QComboBox();
    videoLineEdit = new QComboBox();
    docLineEdit = new QComboBox();

    picLineEdit->setEditable(true);
    picLineEdit->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:300px; background-color:white;}"
                            "QComboBox QAbstractItemView::item{height:22px;}"
                            "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                            "QComboBox::drop-down{border:0px;}");
    musicLineEdit->setEditable(true);
    musicLineEdit->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:300px; background-color:white;}"
                            "QComboBox QAbstractItemView::item{height:22px;}"
                            "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                            "QComboBox::drop-down{border:0px;}");
    videoLineEdit->setEditable(true);
    videoLineEdit->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:300px; background-color:white;}"
                            "QComboBox QAbstractItemView::item{height:22px;}"
                            "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                            "QComboBox::drop-down{border:0px;}");
    docLineEdit->setEditable(true);
    docLineEdit->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:300px; background-color:white;}"
                            "QComboBox QAbstractItemView::item{height:22px;}"
                            "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                            "QComboBox::drop-down{border:0px;}");

    restoreBtn = new QPushButton();
    picBtn = new QPushButton();
    musicBtn = new QPushButton() ;
    videoBtn = new QPushButton();
    docBtn = new QPushButton();
    restoreBtn->setStyleSheet("QPushButton{border:0px; width:100px; height:18px; color:rgb(51, 168, 235); background-color:white;}"
                              "QPushButton:hover{border:0px; color:rgb(51, 168, 235); background-color:rgb(244, 244, 244);}");
    restoreBtn->setText(tr("恢复默认路径"));
    picBtn->setFixedSize(30, 24);
    musicBtn->setFixedSize(30, 24);
    videoBtn->setFixedSize(30, 24);
    docBtn->setFixedSize(30, 24);
    picBtn->setStyleSheet("QPushButton{width:30px; height:25px; border-image:url(:/icon/file);}"
                          "QPushButton:hover{width:30px; height:25px; border-image:url(:/icon/dfile);}"
                          "QPushButton:pressed{width:30px; height:25px; border-image:url(:/icon/dfile);}");
    musicBtn->setStyleSheet("QPushButton{width:30px; height:28px; border-image:url(:/icon/file);}"
                          "QPushButton:hover{width:30px; height:25px; border-image:url(:/icon/dfile);}"
                          "QPushButton:pressed{width:30px; height:25px; border-image:url(:/icon/dfile);}");
    videoBtn->setStyleSheet("QPushButton{width:30px; height:25px; border-image:url(:/icon/file);}"
                          "QPushButton:hover{width:30px; height:25px; border-image:url(:/icon/dfile);}"
                          "QPushButton:pressed{width:30px; height:25px; border-image:url(:/icon/dfile);}");
    docBtn->setStyleSheet("QPushButton{width:30px; height:25px; border-image:url(:/icon/file);}"
                          "QPushButton:hover{width:30px; height:25px; border-image:url(:/icon/dfile);}"
                          "QPushButton:pressed{width:30px; height:25px; border-image:url(:/icon/dfile);}");

    QHBoxLayout *hlayout1 = new QHBoxLayout();
    QHBoxLayout *hlayout2 = new QHBoxLayout();
    QHBoxLayout *hlayout3 = new QHBoxLayout();
    QHBoxLayout *hlayout4 = new QHBoxLayout();
    QHBoxLayout *toplayout = new QHBoxLayout();
    QVBoxLayout *vlayout = new QVBoxLayout();

    toplayout->addWidget(restoreBtn, 0, Qt::AlignRight);
    toplayout->setContentsMargins(0, 0, 20, 0);
    hlayout1->addWidget(picLabel);
    hlayout1->addWidget(picLineEdit);
    hlayout1->addWidget(picBtn);
    hlayout1->addSpacing(20);
    hlayout2->addWidget(musicLabel);
    hlayout2->addWidget(musicLineEdit);
    hlayout2->addWidget(musicBtn);
    hlayout2->addSpacing(20);
    hlayout3->addWidget(videoLabel);
    hlayout3->addWidget(videoLineEdit);
    hlayout3->addWidget(videoBtn);
    hlayout3->addSpacing(20);
    hlayout4->addWidget(docLabel);
    hlayout4->addWidget(docLineEdit);
    hlayout4->addWidget(docBtn);
    hlayout4->addSpacing(20);

    vlayout->addLayout(toplayout);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout4);
    vlayout->addSpacing(20);
    vlayout->setContentsMargins(0, 0, 0, 0);
    srcWidget->setLayout(vlayout);
    /*下一级设置界面接口*/
    connect(picBtn, SIGNAL(clicked(bool)),this, SLOT(switchPicSetting()));
    connect(musicBtn, SIGNAL(clicked(bool)),this, SLOT(switchMusSetting()));
    connect(videoBtn, SIGNAL(clicked(bool)),this, SLOT(switchVidSetting()));
    connect(docBtn, SIGNAL(clicked(bool)),this, SLOT(switchDocSetting()));
    connect(restoreBtn, SIGNAL(clicked(bool)), this, SLOT(restoreIniFile()));
}


void SettingDlg::initToWidget()
{
    toWidget = new QWidget();
    QLabel *toDirLabel = new QLabel();
    toDirLineEdit = new QComboBox();
    toDirBtn = new QPushButton();
    restoreToBtn = new QPushButton();
    toDirLineEdit->setEditable(true);
    toDirLineEdit->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:300px; background-color:white;}"
                                 "QComboBox QAbstractItemView::item{height:22px;}"
                                 "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                                 "QComboBox::drop-down{border:0px;}");
    toDirLabel->setStyleSheet("QLabel{color:gray;font-size:14px; font-family:微软雅黑;}");
    toDirLabel->setText(tr("目标路径"));
    toDirBtn->setFixedSize(30, 24);
    toDirBtn->setStyleSheet("QPushButton{width:30px; height:25px; border-image:url(:/icon/file);}"
                            "QPushButton:hover{width:30px; height:25px; border-image:url(:/icon/dfile);}"
                            "QPushButton:pressed{width:30px; height:25px; border-image:url(:/icon/dfile);}");
    restoreToBtn->setStyleSheet("QPushButton{border:0px; width:100px; height:18px; color:rgb(51, 168, 235); background-color:white;}"
                              "QPushButton:hover{border:0px; color:rgb(51, 168, 235); background-color:rgb(244, 244, 244);}");
    restoreToBtn->setText(tr("恢复默认路径"));
    QHBoxLayout *topHlayout = new QHBoxLayout();
    QHBoxLayout *hlayout1 = new QHBoxLayout();
    QVBoxLayout *toMainLayout = new QVBoxLayout();
    topHlayout->addWidget(restoreToBtn, 0, Qt::AlignRight);
    topHlayout->setContentsMargins(0, 6, 20, 0);
    hlayout1->addWidget(toDirLabel);
    hlayout1->addWidget(toDirLineEdit);
    hlayout1->addWidget(toDirBtn);
    hlayout1->addSpacing(20);
    toMainLayout->addLayout(topHlayout);
    toMainLayout->addLayout(hlayout1);
    toMainLayout->addStretch();
    toWidget->setLayout(toMainLayout);    
    connect(toDirBtn, SIGNAL(clicked(bool)), this, SLOT(setupDstDir()));
    connect(restoreToBtn, SIGNAL(clicked(bool)), this, SLOT(restoreIniFile()));
}




void SettingDlg::readInitDisplay()
{
    /*清除Commbox中现有的内容，重新读取并设置*/
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->beginGroup(DOCUMENT_GROUNP);
    QStringList docPath = appSetting->allKeys();
    QString singleDocPath;
    foreach (singleDocPath, docPath)
    {
        docLineEdit->addItem(singleDocPath);
    }
    appSetting->endGroup();

    appSetting->beginGroup(PICTURE_GROUNP);
    QStringList picPath = appSetting->allKeys();
    QString singlePicPath;
    foreach (singlePicPath, picPath)
    {
        picLineEdit->addItem(singlePicPath);
    }
    appSetting->endGroup();
    appSetting->beginGroup(VIDEO_GROUNP);
    QStringList vidPath = appSetting->allKeys();
    QString singleVidPath;
    foreach (singleVidPath, vidPath)
    {
        videoLineEdit->addItem(singleVidPath);
    }
    appSetting->endGroup();
    appSetting->beginGroup(MUSIC_GROUNP);
    QStringList musPath = appSetting->allKeys();
    QString singleMusPath;
    foreach (singleMusPath, musPath)
    {
        musicLineEdit->addItem(singleMusPath);
    }
    appSetting->endGroup();
    appSetting->beginGroup(DESTINATION_GROUNP);
    QString dstPath = appSetting->value("DEFAULT").toString();
    toDirLineEdit->addItem(dstPath);
    appSetting->endGroup();
    delete appSetting;
}

void SettingDlg::setupDstDir()
{

    toDirLineEdit->clear();
    //toDirLineEdit->removeItem(0);
    QString tmpDstDir = QFileDialog::getExistingDirectory(this, tr("选择源路径"),
                                                       "D:\\",
                                                       QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->beginGroup(DESTINATION_GROUNP);
    qDebug()<<appSetting->fileName();
    if (tmpDstDir.isEmpty())
    {
        QString dstPath = appSetting->value(DEFAULT).toString();
        toDirLineEdit->addItem(dstPath);
    }
    else
    {
        tmpDstDir.replace("/","\\");
        toDirLineEdit->addItem(tmpDstDir);
        appSetting->setValue(DEFAULT, tmpDstDir);
    }
    appSetting->endGroup();
    delete appSetting;
    emit modifySetting();
}

void SettingDlg::switchPicSetting()
{
    picDlg->exec();
}

void SettingDlg::switchMusSetting()
{
    musDlg->exec();
}

void SettingDlg::switchDocSetting()
{
    docDlg->exec();
}

void SettingDlg::switchVidSetting()
{
    vidDlg->exec();
}

void SettingDlg::readUpdateDisplay()
{
    /*清除commbox原有的item*/
    docLineEdit->clear();
    picLineEdit->clear();
    videoLineEdit->clear();
    musicLineEdit->clear();

    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->beginGroup(DOCUMENT_GROUNP);
    QStringList docPath = appSetting->allKeys();
    QString singleDocPath;
    foreach (singleDocPath, docPath)
    {
        docLineEdit->addItem(singleDocPath);
    }
    appSetting->endGroup();

    appSetting->beginGroup(PICTURE_GROUNP);
    QStringList picPath = appSetting->allKeys();
    QString singlePicPath;
    foreach (singlePicPath, picPath)
    {
        picLineEdit->addItem(singlePicPath);
    }
    appSetting->endGroup();
    appSetting->beginGroup(VIDEO_GROUNP);
    QStringList vidPath = appSetting->allKeys();
    QString singleVidPath;
    foreach (singleVidPath, vidPath)
    {
        videoLineEdit->addItem(singleVidPath);
    }
    appSetting->endGroup();
    appSetting->beginGroup(MUSIC_GROUNP);
    QStringList musPath = appSetting->allKeys();
    QString singleMusPath;
    foreach (singleMusPath, musPath)
    {
        musicLineEdit->addItem(singleMusPath);
    }
    appSetting->endGroup();
    appSetting->beginGroup(DESTINATION_GROUNP);
    QString dstPath = appSetting->value("DEFAULT").toString();
    toDirLineEdit->addItem(dstPath);
    appSetting->endGroup();
    delete appSetting;
}

void SettingDlg::restoreIniFile()
{
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->clear();
    delete appSetting;

    ConfigureDirTool sysConf("NFSChina", "DataMigration");
    sysConf.createIniFile();
}




