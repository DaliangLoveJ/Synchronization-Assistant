#include "windows/directorydlg.h"
#include "functions/common.h"
#include <QPixmap>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QListView>
#include <QFileDialog>


DirectoryDlg::DirectoryDlg(QString userGroup, QWidget *parent)
    : QDialog(parent)
{
    resize(520, 280);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowIcon(QIcon(":/title/logo"));
    user_group = userGroup;
    QLabel *titleIcon = new QLabel();
    QLabel *titleLabel = new QLabel();
    QPushButton *closeButton = new QPushButton();
    QPushButton *addPath = new QPushButton();
    QPushButton *removePath = new QPushButton();
    cancelBtn = new QPushButton();
    QLabel *prompt = new QLabel();    
    this->readIniFile();
    listItemModel = new QStandardItemModel(this);
    leftPathView = new QListView();
    leftPathView->setContentsMargins(0, 0, 0, 0);
    leftPathView->setStyleSheet("QListView{border:none; background:rgb(230,240,230);}");
    this->createPathListView();
    prompt->setStyleSheet("QLabel{color:gray;}");
    prompt->setText(tr("源路径列表"));
    prompt->setContentsMargins(10, 0, 0, 0);
    addPath->setFixedWidth(80);
    removePath->setFixedWidth(80);
    cancelBtn->setFixedWidth(80);
    addPath->setStyleSheet("QPushButton{border-radius:5px; border:1px solid lightgray; background:rgb(230,230,230);height:23px}"
                             "QPushButton:hover{border-radius:5px; border-color:green; background:transparent;}");
    removePath->setStyleSheet("QPushButton{border-radius:5px; border:1px solid lightgray; background:rgb(230,230,230);height:23px}"
                              "QPushButton:hover{border-radius:5px; border-color:green; background:transparent;}");
    cancelBtn->setStyleSheet("QPushButton{border-radius:5px; border:1px solid lightgray; background:rgb(230,230,230);height:23px}"
                             "QPushButton:hover{border-radius:5px; border-color:green; background:transparent;}");
    addPath->setText(tr("增加"));
    removePath->setText(tr("移除"));
    cancelBtn->setText(tr("返回"));
    closeButton->setFixedSize(18, 18);
    closeButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/close);}"
                               "QPushButton:hover{border-radius:5px; border-image:url(:/title/dclose);}"
                               "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dclose);}");

    titleLabel->setStyleSheet("QLabel{color:white;}");
    titleLabel->setText(tr("数据迁移-源路径设置"));
    titleIcon->setPixmap(QPixmap(":/title/logo"));
    titleIcon->setFixedSize(16, 16);
    titleIcon->setScaledContents(true);
    closeButton->setContentsMargins(0, 0, 0, 5);
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIcon, 0, Qt::AlignVCenter);
    titleLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton, 0, Qt::AlignVCenter);
    titleLayout->setSpacing(5);
    titleLayout->setContentsMargins(4, 0, 4, 0);
    QHBoxLayout *prompyHlayout = new QHBoxLayout();
    prompyHlayout->addWidget(prompt);
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addSpacing(50);
    buttonLayout->addWidget(addPath);
    buttonLayout->addWidget(removePath);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->setSpacing(15);
    buttonLayout->addStretch();
    QHBoxLayout *centerHlayout = new QHBoxLayout();
    centerHlayout->addWidget(leftPathView);
    centerHlayout->addSpacing(8);
    centerHlayout->addLayout(buttonLayout);
    centerHlayout->addSpacing(10);
    centerHlayout->setContentsMargins(2, 0, 0, 0);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(prompyHlayout);
    mainLayout->addLayout(centerHlayout);
    mainLayout->addSpacing(0);
    mainLayout->setContentsMargins(0, 6, 0, 0);
    setLayout(mainLayout);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(addPath, SIGNAL(clicked()), this, SLOT(addPathSlot()));
    connect(removePath, SIGNAL(clicked()), this, SLOT(removePathSlot()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtnSlot()));

}

DirectoryDlg::~DirectoryDlg()
{
}

void DirectoryDlg::paintEvent(QPaintEvent *)
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


void DirectoryDlg::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        mouse_press = true;
    }
    move_point = event->globalPos() - pos();
}

void DirectoryDlg::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}

void DirectoryDlg::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_press)
    {
        QPoint move_pos = event->globalPos();
        move(move_pos - move_point);
    }
}

void DirectoryDlg::readIniFile()
{
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->beginGroup(user_group);
    allPath = appSetting->allKeys();
    appSetting->endGroup();
    delete appSetting;
}

bool DirectoryDlg::createPathListView()
{
    if (allPath.isEmpty())
        return false;

    for( int i=0; i< allPath.size(); i++)
    {
        QString singlePath = static_cast<QString>(allPath.at(i));
        QStandardItem *pathItem = new QStandardItem(singlePath);
        listItemModel->appendRow(pathItem);
    }
    leftPathView->setModel(listItemModel);
    leftPathView->setFixedSize(460, 220);
}

void DirectoryDlg::addPathSlot()
{
    QString addDir = QFileDialog::getExistingDirectory(this, tr("选择源路径"),
                                                       "../",
                                                       QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->beginGroup(user_group);
    if (!addDir.isEmpty())
    {
        addDir.replace("/","\\");
        QStandardItem *addPathItem = new QStandardItem(addDir);
        listItemModel->appendRow(addPathItem);
        appSetting->setValue(addDir, CUSTOM);

    }
    appSetting->endGroup();
    delete appSetting;

    emit iniFileModify();
}

void DirectoryDlg::removePathSlot()
{
    currentIndex = leftPathView->currentIndex();
    int currentRow = currentIndex.row();
    QString currentPath = currentIndex.data().toString();
    listItemModel->removeRow(currentRow);
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    appSetting->beginGroup(user_group);
    if (appSetting->childKeys().contains(currentPath))
    {
        appSetting->remove(currentPath);
    }
    appSetting->endGroup();
    delete appSetting;

    emit iniFileModify();
}

void DirectoryDlg::cancelBtnSlot()
{
    this->hide();
}









