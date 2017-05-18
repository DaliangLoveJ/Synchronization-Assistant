#include "windows/userdetailwidget.h"
#include "windows/userdatamodel.h"
#include "windows/messagebox.h"
#include "functions/common.h"
#include "tools/utils.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollBar>
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
#include <QTableView>
#include <QHeaderView>
#include <QCheckBox>
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProgressBar>
#include "JlCompress.h"


UserDetailWidget::UserDetailWidget(QWidget *parent)
    :QDialog(parent)
{
    /*用于从配置文件中获取文件路径，必须要调用*/
    getInitFilePath();
    msg_doc = new CustomMessageBox();
    msg_mus = new CustomMessageBox();
    msg_vid = new CustomMessageBox();
    msg_pic = new CustomMessageBox();
    th_doc = new QThread();
    userdata_doc = new UserData();
    connect(userdata_doc,SIGNAL(UserData_error_name(int,QString)),this,SLOT(UserData_error_name_Slot(int,QString)));
    connect(userdata_doc,SIGNAL(UserData_over_name(int,QString)),this,SLOT(UserData_over_name_slot(int,QString)));
    connect(userdata_doc,SIGNAL(UserData_copy_num(int,int)),this,SLOT(UserData_copy_num_slot(int,int)));
    connect(userdata_doc,SIGNAL(UserData_current_copy_name(int,QString)),this,SLOT(UserData_current_copy_name_slot(int,QString)));
    userdata_doc->moveToThread(th_doc);
    connect(th_doc,SIGNAL(started()),userdata_doc,SLOT(UserDataCopyToPath()));

    th_mus = new QThread();
    userdata_mus = new UserData();
    connect(userdata_mus,SIGNAL(UserData_error_name(int,QString)),this,SLOT(UserData_error_name_Slot(int,QString)));
    connect(userdata_mus,SIGNAL(UserData_over_name(int,QString)),this,SLOT(UserData_over_name_slot(int,QString)));
    connect(userdata_mus,SIGNAL(UserData_copy_num(int,int)),this,SLOT(UserData_copy_num_slot(int,int)));
    connect(userdata_mus,SIGNAL(UserData_current_copy_name(int,QString)),this,SLOT(UserData_current_copy_name_slot(int,QString)));
    userdata_mus->moveToThread(th_mus);
    connect(th_mus,SIGNAL(started()),userdata_mus,SLOT(UserDataCopyToPath()));

    th_vid = new QThread();
    userdata_vid = new UserData();
    connect(userdata_vid,SIGNAL(UserData_error_name(int,QString)),this,SLOT(UserData_error_name_Slot(int,QString)));
    connect(userdata_vid,SIGNAL(UserData_over_name(int,QString)),this,SLOT(UserData_over_name_slot(int,QString)));
    connect(userdata_vid,SIGNAL(UserData_copy_num(int,int)),this,SLOT(UserData_copy_num_slot(int,int)));
    connect(userdata_vid,SIGNAL(UserData_current_copy_name(int,QString)),this,SLOT(UserData_current_copy_name_slot(int,QString)));
    userdata_vid->moveToThread(th_vid);
    connect(th_vid,SIGNAL(started()),userdata_vid,SLOT(UserDataCopyToPath()));

    th_pic = new QThread();
    userdata_pic = new UserData();
    connect(userdata_pic,SIGNAL(UserData_error_name(int,QString)),this,SLOT(UserData_error_name_Slot(int,QString)));
    connect(userdata_pic,SIGNAL(UserData_over_name(int,QString)),this,SLOT(UserData_over_name_slot(int,QString)));
    connect(userdata_pic,SIGNAL(UserData_copy_num(int,int)),this,SLOT(UserData_copy_num_slot(int,int)));
    connect(userdata_pic,SIGNAL(UserData_current_copy_name(int,QString)),this,SLOT(UserData_current_copy_name_slot(int,QString)));
    userdata_pic->moveToThread(th_pic);
    connect(th_pic,SIGNAL(started()),userdata_pic,SLOT(UserDataCopyToPath()));

    th_one = new QThread();
    userdata_one = new UserData();
    userdata_one->moveToThread(th_one);
    connect(userdata_one,SIGNAL(UserData_copy_num(int,int)),this,SLOT(UserData_one_overSlot(int,int)));
    connect(userdata_one,SIGNAL(UserData_progress_num(int,int,int)),this,SLOT(UserData_progress_num_slot(int,int,int)));
    connect(th_one,SIGNAL(started()),userdata_one,SLOT(UserDataCopyToPath()));


    setWindowFlags(Qt::FramelessWindowHint);
    leftListWidget = new QListWidget();
    rightStackWidget = new QStackedWidget();
    QListWidgetItem *picDetalItem = new QListWidgetItem(tr("图片"));
    QListWidgetItem *vidDetailItem = new QListWidgetItem(tr("视频"));
    QListWidgetItem *musDetailItem = new QListWidgetItem(tr("音乐"));
    QListWidgetItem *docDetailItem = new QListWidgetItem(tr("文档"));
    picDetalItem->setTextAlignment(Qt::AlignCenter);
    vidDetailItem->setTextAlignment(Qt::AlignCenter);
    musDetailItem->setTextAlignment(Qt::AlignCenter);
    docDetailItem->setTextAlignment(Qt::AlignCenter);
    leftListWidget->setFixedWidth(97);
    leftListWidget->insertItem(0, picDetalItem);
    leftListWidget->insertItem(1, vidDetailItem);
    leftListWidget->insertItem(2, musDetailItem);
    leftListWidget->insertItem(3, docDetailItem);
    leftListWidget->setStyleSheet("QListWidget{border:0px;background-color:rgb(244, 244, 244)}"
                                  "QListWidget::item{color:rgb(101, 101, 101); height: 40px;}"
                                  "QListWidget::item:hover{color:white; background-color:rgb(51, 200, 235);}"
                                  "QListWidget::item:selected{border:0px; color:white; background-color:rgb(51, 168, 235);}");
    leftListWidget->setCurrentRow(0);
    leftListWidget->setFocusPolicy(Qt::NoFocus);
    initPicWidget();
    initVidWidget();
    initMusWidget();
    initDocWidget();
    rightStackWidget->insertWidget(0, picDetalWidget);
    rightStackWidget->insertWidget(1, vidDetailWidget);
    rightStackWidget->insertWidget(2, musDetailWidget);
    rightStackWidget->insertWidget(3, docDetailWidget);
    QHBoxLayout *centerHlayout = new QHBoxLayout();
    centerHlayout->addWidget(leftListWidget, 0, Qt::AlignLeft);
    centerHlayout->addWidget(rightStackWidget);
    centerHlayout->setSpacing(0);
    centerHlayout->setContentsMargins(1, 0, 0, 1);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(centerHlayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
    connect(leftListWidget, SIGNAL(currentRowChanged(int)), rightStackWidget,SLOT(setCurrentIndex(int)));
    connect(leftListWidget, SIGNAL(currentRowChanged(int)), this,SLOT(updateDataModel()));

    connect(msg_doc, SIGNAL(ensureBtnClicked()), this, SLOT(compressData())) ;
    connect(msg_mus, SIGNAL(ensureBtnClicked()), this, SLOT(compressData())) ;
    connect(msg_vid, SIGNAL(ensureBtnClicked()), this, SLOT(compressData())) ;
    connect(msg_pic, SIGNAL(ensureBtnClicked()), this, SLOT(compressData())) ;


}

UserDetailWidget::~UserDetailWidget()
{

}

void UserDetailWidget::paintEvent(QPaintEvent *)
{
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
    painterLeft.drawRect(QRect(0, 0, 97, this->height()-5));
    QPainter painterFrame(this);
    painterFrame.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 0), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 0)};
    painterFrame.drawPolyline(points, 4);

}

void UserDetailWidget::translateLanguage()
{
}

void UserDetailWidget::initPicWidget()
{
    picDetalWidget = new QWidget();
    picTableView = new QTableView(this);
    picModel     = new TableModel(this);
    picDelegate = new CheckBoxDelegate(this);
    picTableView->setStyleSheet("QTableView{border:none; background: white;}"
                                "QTableView::item {color:rgb(50, 50, 50); border: none; border-bottom: 0px solid rgb(50, 50, 50);}"
                                "QTableView::item:selected {background: rgb(0, 160, 230);}");
    picTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    picTableView->horizontalHeader()->setStretchLastSection(true);
    picTableView->horizontalHeader()->setHighlightSections(false);
    picTableView->verticalHeader()->setVisible(false);
    picTableView->setShowGrid(false);
    picTableView->setFrameShape(QFrame::NoFrame);
    picTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{height:26px; border:1px solid #333333; border-color:white; background-color:rgb(244, 244, 244);}");
    picTableView->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                       "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                       "QScrollBar::handle:hover{background:gray;}"
                                                       "QScrollBar::sub-line{background:transparent;}"
                                                       "QScrollBar::add-line{background:transparent;}");

    picTableView->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                     "QScrollBar::handle:hover{background:gray;}"
                                                     "QScrollBar::sub-line{background:transparent;}"
                                                     "QScrollBar::add-line{background:transparent;}");

    picTableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置为单选模式
    picTableView->setModel(picModel);
    picTableView->setItemDelegate(picDelegate);
    picTableView->setColumnWidth(0,100);
    picTableView->setColumnWidth(1,100);
    picTableView->setColumnWidth(2,100);
    picTableView->setColumnWidth(3,100);
    picTableView->setColumnWidth(4,320);

    QLabel *picCheckPrompt = new QLabel();
    QPushButton *picStartExport = new QPushButton();
    picCheckPrompt->setStyleSheet("QLabel{color:gray;}");
    picCheckPrompt->setText(tr("检测到以下数据可以选择导出"));
    picStartExport->setFixedSize(90, 28);
    picStartExport->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/dstartexport); color:gray;}"
                                  "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/startexport); color:gray;}"
                                  "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/ddstartexport); color:gray;}");

    picProgressBar = new QProgressBar();
    picProgressBar->setStyleSheet("QProgressBar{border:none; color:white; text-align:center; background:white; }"
                                  "QProgressBar::chunk{background: rgb(0, 160, 230); }");

    picProgressBar->setRange(0,100);
    picAllchecked = new QCheckBox();
    picAllchecked->setText(tr("全选"));
    picExportToPath = new QPushButton(); //背景设置为透明或者白色，前景色设置为蓝色，显示和更改导出路径
    QLabel *picExportToLabel = new QLabel();
    picAllchecked->setStyleSheet("QCheckBox{color:gray;}");
    picExportToLabel->setStyleSheet("QLabel{color:gray;}");
    picExportToPath->setStyleSheet("QPushButton{border:none; color:rgb(51, 168, 235); background:white;}");
    picExportToLabel->setText(tr("导出到"));
    picExportToPath->setText(dstDirPath);
    QVBoxLayout *picMainLayout = new QVBoxLayout();
    QHBoxLayout *picTopMainLayout = new QHBoxLayout();
    QHBoxLayout *picCenterMainLayout = new QHBoxLayout();
    QHBoxLayout *picBottomMainLayout = new QHBoxLayout();
    picTopMainLayout->addWidget(picCheckPrompt);

    picTopMainLayout->addSpacing(10);
    picTopMainLayout->addWidget(picProgressBar);
    picTopMainLayout->addSpacing(10);
    picTopMainLayout->addWidget(picStartExport);
    picTopMainLayout->setContentsMargins(10, 10, 10, 5);
    picCenterMainLayout->addWidget(picTableView);
    picBottomMainLayout->addWidget(picAllchecked);
    picBottomMainLayout->addSpacing(80);
    picBottomMainLayout->addWidget(picExportToLabel);
    picBottomMainLayout->addWidget(picExportToPath);
    picBottomMainLayout->addStretch();
    picBottomMainLayout->setContentsMargins(43, 10, 0, 5);
    picBottomMainLayout->setSpacing(4);
    picMainLayout->setMargin(0);
    picMainLayout->addLayout(picTopMainLayout);
    picMainLayout->addLayout(picCenterMainLayout);
    picMainLayout->addLayout(picBottomMainLayout);
    picMainLayout->setSpacing(0);
    picMainLayout->setContentsMargins(0, 0, 1, 0);
    picDetalWidget->setLayout(picMainLayout);

    connect(picTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(getPicCheckedSlot(QModelIndex)));
    connect(picAllchecked, SIGNAL(stateChanged(int)),this, SLOT(picCheckedBoxAll(int)));
    connect(picStartExport, SIGNAL(clicked()), this, SLOT(exportPicSlot()));

}

void UserDetailWidget::initVidWidget()
{

    vidDetailWidget = new QWidget();    
    vidTableView = new QTableView(this);
    vidModel = new TableModel(this);
    vidDelegate = new CheckBoxDelegate(this);
    vidTableView->setStyleSheet("QTableView{border:0px solid rgb(50, 50, 50); background: rgb(255, 255, 255);color: rgb(50, 50, 50);}"
                               "QTableView::item {color:rgb(50, 50, 50); border: none;border-bottom: 0px solid rgb(50, 50, 50);}"
                               "QTableView::item:selected {background: rgb(0, 160, 230);}");

    vidTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    vidTableView->horizontalHeader()->setStretchLastSection(true);
    vidTableView->horizontalHeader()->setHighlightSections(false);
    vidTableView->verticalHeader()->setVisible(false);
    vidTableView->setShowGrid(false);
    vidTableView->setFrameShape(QFrame::NoFrame);
    vidTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{height:26px; border:1px solid #333333; border-color:white; background-color:rgb(244, 244, 244);}");
    vidTableView->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                       "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                       "QScrollBar::handle:hover{background:gray;}"
                                                       "QScrollBar::sub-line{background:transparent;}"
                                                       "QScrollBar::add-line{background:transparent;}");

    vidTableView->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                     "QScrollBar::handle:hover{background:gray;}"
                                                     "QScrollBar::sub-line{background:transparent;}"
                                                     "QScrollBar::add-line{background:transparent;}");

    vidTableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置为单选模式
    vidTableView->setModel(vidModel);
    vidTableView->setItemDelegate(vidDelegate);
    vidTableView->setColumnWidth(0,100);
    vidTableView->setColumnWidth(1,100);
    vidTableView->setColumnWidth(2,100);
    vidTableView->setColumnWidth(3,100);
    vidTableView->setColumnWidth(4,320);
    QLabel *vidCheckPrompt = new QLabel();
    QPushButton *vidStartExport = new QPushButton();
    vidCheckPrompt->setStyleSheet("QLabel{color:gray;}");
    vidCheckPrompt->setText(tr("检测到以下数据可以选择导出"));
    vidStartExport->setFixedSize(90, 28);
    vidStartExport->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/dstartexport); color:gray;}"
                                  "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/startexport); color:gray;}"
                                  "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/ddstartexport); color:gray;}");

    vidProgressBar = new QProgressBar();
    vidProgressBar->setStyleSheet("QProgressBar{border:none; color:white; text-align:center; background:white; }"
                                  "QProgressBar::chunk{background: rgb(0, 160, 230); }");

    vidProgressBar->setRange(0,100);
    vidAllchecked = new QCheckBox();
    vidAllchecked->setText(tr("全选"));
    vidExportToPath = new QPushButton();
    QLabel *vidExportToLabel = new QLabel();
    vidAllchecked->setStyleSheet("QCheckBox{color:gray;}");
    vidExportToLabel->setStyleSheet("QLabel{color:gray;}");
    vidExportToPath->setStyleSheet("QPushButton{border:none; color:rgb(51, 168, 235); background:white;}");
    vidExportToLabel->setText(tr("导出到"));
    vidExportToPath->setText(dstDirPath);

    QVBoxLayout *vidMainLayout = new QVBoxLayout();
    QHBoxLayout *vidTopMainLayout = new QHBoxLayout();
    QHBoxLayout *vidCenterMainLayout = new QHBoxLayout();
    QHBoxLayout *vidBottomMainLayout = new QHBoxLayout();
    vidTopMainLayout->addWidget(vidCheckPrompt);
    vidTopMainLayout->addSpacing(10);
    vidTopMainLayout->addWidget(vidProgressBar);
    vidTopMainLayout->addSpacing(10);
    vidTopMainLayout->addWidget(vidStartExport);
    vidTopMainLayout->setContentsMargins(10, 10, 10, 5);
    vidCenterMainLayout->addWidget(vidTableView);

    vidBottomMainLayout->addWidget(vidAllchecked);
    vidBottomMainLayout->addSpacing(80);
    vidBottomMainLayout->addWidget(vidExportToLabel);
    vidBottomMainLayout->addWidget(vidExportToPath);
    vidBottomMainLayout->addStretch();
    vidBottomMainLayout->setContentsMargins(50, 0, 0, 5);
    vidBottomMainLayout->setSpacing(4);

    vidMainLayout->setMargin(0);
    vidMainLayout->addLayout(vidTopMainLayout);
    vidMainLayout->addLayout(vidCenterMainLayout);
    vidMainLayout->addLayout(vidBottomMainLayout);
    vidMainLayout->setSpacing(0);
    vidMainLayout->setContentsMargins(0, 0, 1, 0);
    vidDetailWidget->setLayout(vidMainLayout);

    connect(vidTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(getVidCheckedSlot(QModelIndex)));
    connect(vidAllchecked, SIGNAL(stateChanged(int)),this, SLOT(vidCheckedBoxAll(int)));
    connect(vidStartExport, SIGNAL(clicked()), this, SLOT(exportVidSlot()));
}
void UserDetailWidget::initMusWidget()
{
    musDetailWidget = new QWidget();
    musTableView = new QTableView(this);
    musModel = new TableModel(this);
    musDelegate = new CheckBoxDelegate(this);
    musTableView->setStyleSheet("QTableView{border:0px solid rgb(50, 50, 50); background: rgb(255, 255, 255);color: rgb(50, 50, 50);}"
                                "QTableView::item {color:rgb(50, 50, 50); border: none;border-bottom: 0px solid rgb(50, 50, 50);}"
                                "QTableView::item:selected {background: rgb(0, 160, 230);}");

    musTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    musTableView->horizontalHeader()->setStretchLastSection(true);
    musTableView->horizontalHeader()->setHighlightSections(false);
    musTableView->verticalHeader()->setVisible(false);
    musTableView->setShowGrid(false);
    musTableView->setFrameShape(QFrame::NoFrame);
    musTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{height:26px; border:1px solid #333333; border-color:white; background-color:rgb(244, 244, 244);}");
    musTableView->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                       "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                       "QScrollBar::handle:hover{background:gray;}"
                                                       "QScrollBar::sub-line{background:transparent;}"
                                                       "QScrollBar::add-line{background:transparent;}");

    musTableView->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                     "QScrollBar::handle:hover{background:gray;}"
                                                     "QScrollBar::sub-line{background:transparent;}"
                                                     "QScrollBar::add-line{background:transparent;}");

    musTableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置为单选模式
    musTableView->setModel(musModel);
    musTableView->setItemDelegate(musDelegate);
    musTableView->setColumnWidth(0,100);
    musTableView->setColumnWidth(1,100);
    musTableView->setColumnWidth(2,100);
    musTableView->setColumnWidth(3,100);
    musTableView->setColumnWidth(4,320);
    QLabel *musCheckPrompt = new QLabel();
    QPushButton *musStartExport = new QPushButton();
    musCheckPrompt->setStyleSheet("QLabel{color:gray;}");
    musCheckPrompt->setText(tr("检测到以下数据可以选择导出"));
    musStartExport->setFixedSize(90, 28);
    musStartExport->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/dstartexport); color:gray;}"
                                  "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/startexport); color:gray;}"
                                  "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/ddstartexport); color:gray;}");

    musProgressBar = new QProgressBar();
    musProgressBar->setStyleSheet("QProgressBar{border:none; color:white; text-align:center; background:white; }"
                                   "QProgressBar::chunk{background: rgb(0, 160, 230); }");

    musProgressBar->setRange(0,100);
    musAllchecked = new QCheckBox();
    musAllchecked->setText(tr("全选"));
    musExportToPath = new QPushButton();
    QLabel *musExportToLabel = new QLabel();
    musAllchecked->setStyleSheet("QCheckBox{color:gray;}");
    musExportToLabel->setStyleSheet("QLabel{color:gray;}");
    musExportToLabel->setText(tr("导出到"));
    musExportToPath->setStyleSheet("QPushButton{border:none; color:rgb(51, 168, 235); background:white;}");
    musExportToPath->setText(dstDirPath);

    QVBoxLayout *musMainLayout = new QVBoxLayout();
    QHBoxLayout *musTopMainLayout = new QHBoxLayout();
    QHBoxLayout *musCenterMainLayout = new QHBoxLayout();
    QHBoxLayout *musBottomMainLayout = new QHBoxLayout();
    musTopMainLayout->addWidget(musCheckPrompt);
    musTopMainLayout->addSpacing(10);
    musTopMainLayout->addWidget(musProgressBar);
    musTopMainLayout->addSpacing(10);
    musTopMainLayout->addWidget(musStartExport);
    musTopMainLayout->setContentsMargins(10, 10, 10, 5);
    musCenterMainLayout->addWidget(musTableView);
    musBottomMainLayout->addWidget(musAllchecked);
    musBottomMainLayout->addSpacing(80);
    musBottomMainLayout->addWidget(musExportToLabel);
    musBottomMainLayout->addWidget(musExportToPath);
    musBottomMainLayout->addStretch();
    musBottomMainLayout->setContentsMargins(50, 0, 0, 5);
    musBottomMainLayout->setSpacing(4);
    musMainLayout->setMargin(0);
    musMainLayout->addLayout(musTopMainLayout);
    musMainLayout->addLayout(musCenterMainLayout);
    musMainLayout->addLayout(musBottomMainLayout);
    musMainLayout->setSpacing(0);
    musMainLayout->setContentsMargins(0, 0, 1, 0);
    musDetailWidget->setLayout(musMainLayout);

    connect(musTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(getMusicCheckedSlot(QModelIndex)));
    connect(musAllchecked, SIGNAL(stateChanged(int)),this, SLOT(musCheckedBoxAll(int)));
    connect(musStartExport, SIGNAL(clicked()), this, SLOT(exportMusSlot()));
}

void UserDetailWidget::initDocWidget()
{
    docDetailWidget = new QWidget();
    docTableView = new QTableView(this);
    docModel = new TableModel(this);
    docDelegate = new CheckBoxDelegate(this);
    docTableView->setStyleSheet("QTableView{border:0px solid rgb(50, 50, 50); background: rgb(255, 255, 255);color: rgb(50, 50, 50);}"
                               "QTableView::item {color:rgb(50, 50, 50); border: none;border-bottom: 0px solid rgb(50, 50, 50);}"
                               "QTableView::item:selected {background: rgb(0, 160, 230);}");

    docTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    docTableView->horizontalHeader()->setStretchLastSection(true);
    docTableView->horizontalHeader()->setHighlightSections(false);
    docTableView->verticalHeader()->setVisible(false);
    docTableView->setShowGrid(false);
    docTableView->setFrameShape(QFrame::NoFrame);
    docTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{height:26px; border:1px solid #333333; border-color:white; background-color:rgb(244, 244, 244);}");
    docTableView->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                       "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                       "QScrollBar::handle:hover{background:gray;}"
                                                       "QScrollBar::sub-line{background:transparent;}"
                                                       "QScrollBar::add-line{background:transparent;}");

    docTableView->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                     "QScrollBar::handle:hover{background:gray;}"
                                                     "QScrollBar::sub-line{background:transparent;}"
                                                     "QScrollBar::add-line{background:transparent;}");

    docTableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置为单选模式
    docTableView->setModel(docModel);
    docTableView->setItemDelegate(docDelegate);
    docTableView->setColumnWidth(0,100);
    docTableView->setColumnWidth(1,100);
    docTableView->setColumnWidth(2,100);
    docTableView->setColumnWidth(3,100);
    docTableView->setColumnWidth(4,320);
    QLabel *docCheckPrompt = new QLabel();
    QPushButton *docStartExport = new QPushButton();
    docCheckPrompt->setStyleSheet("QLabel{color:gray;}");
    docCheckPrompt->setText(tr("检测到以下数据可以选择导出"));
    docStartExport->setFixedSize(90, 28);
    docStartExport->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/dstartexport); color:gray;}"
                                  "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/startexport); color:gray;}"
                                  "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/ddstartexport); color:gray;}");


    docProgressBar = new QProgressBar();
    docProgressBar->setStyleSheet("QProgressBar{border:none; color:white; text-align:center; background:white; }"
                                  "QProgressBar::chunk{background: rgb(0, 160, 230); }");

    docProgressBar->setRange(0,100);
    docAllchecked = new QCheckBox();
    docAllchecked->setText(tr("全选"));
    docExportToPath = new QPushButton();
    QLabel *docExportToLabel = new QLabel();
    docAllchecked->setStyleSheet("QCheckBox{color:gray;}");
    docExportToLabel->setStyleSheet("QLabel{color:gray;}");
    docExportToPath->setStyleSheet("QPushButton{border:none; color:rgb(51, 168, 235); background:white;}");
    docExportToLabel->setText(tr("导出到"));
    docExportToPath->setText(dstDirPath);
    QVBoxLayout *docMainLayout = new QVBoxLayout();
    QHBoxLayout *docTopMainLayout = new QHBoxLayout();
    QHBoxLayout *docCenterMainLayout = new QHBoxLayout();
    QHBoxLayout *docBottomMainLayout = new QHBoxLayout();
    docTopMainLayout->addWidget(docCheckPrompt);
    docTopMainLayout->addSpacing(10);
    docTopMainLayout->addWidget(docProgressBar);
    docTopMainLayout->addSpacing(10);
    docTopMainLayout->addWidget(docStartExport);
    docTopMainLayout->setContentsMargins(10, 10, 10, 5);
    docCenterMainLayout->addWidget(docTableView);
    docBottomMainLayout->addWidget(docAllchecked);
    docBottomMainLayout->addSpacing(80);
    docBottomMainLayout->addWidget(docExportToLabel);
    docBottomMainLayout->addWidget(docExportToPath);
    docBottomMainLayout->addStretch();
    docBottomMainLayout->setContentsMargins(50, 0, 0, 5);
    docBottomMainLayout->setSpacing(4);
    docMainLayout->setMargin(0);
    docMainLayout->addLayout(docTopMainLayout);
    docMainLayout->addLayout(docCenterMainLayout);
    docMainLayout->addLayout(docBottomMainLayout);
    docMainLayout->setSpacing(0);
    docMainLayout->setContentsMargins(0, 0, 1, 0);
    docDetailWidget->setLayout(docMainLayout);
    connect(docTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(getDocCheckedSlot(QModelIndex)));
    connect(docAllchecked, SIGNAL(stateChanged(int)),this, SLOT(docCheckedBoxAll(int)));
    connect(docStartExport,SIGNAL(clicked()), this, SLOT(exportDocSlot()));
}

void UserDetailWidget::getInitFilePath()
{
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                          QCoreApplication::organizationName(),
                                          QCoreApplication::applicationName());
    appSetting->beginGroup(DESTINATION_GROUNP);
    dstDirPath = appSetting->value(DEFAULT).toString();
    dstDirPath.append("\\");
    appSetting->endGroup();
    appSetting->beginGroup(PICTURE_GROUNP);
    picScanPath = appSetting->allKeys();
    appSetting->endGroup();
    appSetting->beginGroup(DOCUMENT_GROUNP);
    docScanPath = appSetting->allKeys();
    appSetting->endGroup();
    appSetting->beginGroup(MUSIC_GROUNP);
    musScanPath = appSetting->allKeys();
    appSetting->endGroup();
    appSetting->beginGroup(VIDEO_GROUNP);
    vidScanPath = appSetting->allKeys();
    appSetting->endGroup();
    delete appSetting;
}

void UserDetailWidget::updateReadIniFilePath()
{
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                          QCoreApplication::organizationName(),
                                          QCoreApplication::applicationName());
    appSetting->beginGroup(DESTINATION_GROUNP);
    dstDirPath = appSetting->value(DEFAULT).toString();
    picExportToPath->setText(dstDirPath);
    vidExportToPath->setText(dstDirPath);
    musExportToPath->setText(dstDirPath);
    docExportToPath->setText(dstDirPath);
    appSetting->endGroup();
    appSetting->beginGroup(PICTURE_GROUNP);
    picScanPath = appSetting->allKeys();
    appSetting->endGroup();
    appSetting->beginGroup(DOCUMENT_GROUNP);
    docScanPath = appSetting->allKeys();
    appSetting->endGroup();
    appSetting->beginGroup(MUSIC_GROUNP);
    musScanPath = appSetting->allKeys();
    appSetting->endGroup();
    appSetting->beginGroup(VIDEO_GROUNP);
    vidScanPath = appSetting->allKeys();
    appSetting->endGroup();
    delete appSetting;


    dstDirPath = dstDirPath.append("\\");
//    qDebug() << dstDirPath;
    /*建立目标路径*/
    bool   mkDstDirFlag;
    QDir *dstDir = new QDir();
    bool dstExist = dstDir->exists(dstDirPath);
    if (dstExist)
    {
        qDebug() << "user dst dir has been created...";
    }
    else
    {
        mkDstDirFlag = dstDir->mkpath(dstDirPath);
        if(mkDstDirFlag)
        {
            qDebug() << "user dst dir created failed...";
        }
    }
}

void UserDetailWidget::setDataAllModel()
{
    /*搜索图片数据*/
    picProgressBar->hide();
    musProgressBar->hide();
    vidProgressBar->hide();
    docProgressBar->hide();
    QList < QString > pictmpFile;
    QList < QString > mustmpFile;
    QList < QString > vidtmpFile;
    QList < QString > doctmpFile;

    if(!picFileList.isEmpty())
    {
        picFileList.clear();
    }
    if(!musFileList.isEmpty())
    {
        musFileList.clear();
    }
    if(!vidFileList.isEmpty())
    {
        vidFileList.clear();
    }
    if(!docFileList.isEmpty())
    {
        docFileList.clear();
    }
    /*---搜索图片数据------*/
    QString picPath;
    QString picFile;
    QStringList format;
    format<<"*.png"<<"*.jpg"<<"*.bmp"<<"*.jpeg";
    for(int i=0;i<format.count();i++)
    {
        foreach (picPath, picScanPath)
        {
            pictmpFile = Util::findFileList(format.at(i), picPath);
            foreach (picFile, pictmpFile)
            {
                picFileList.append(picFile);
            }
        }
    }
    /*--- end 搜索图片数据------*/
    /*---搜索音乐数据------*/
    QString musPath;
    QString musFile;
    format.clear();
    format<<"*.mp3"<<"*.wav";
    for(int i=0;i<format.count();i++)
    {
        foreach (musPath, musScanPath)
        {
            mustmpFile = Util::findFileList(format.at(i), musPath);
            foreach (musFile, mustmpFile)
            {
                musFileList.append(musFile);
            }
        }
    }
    /*--- end 搜索音乐数据------*/

    /*---搜索文档数据------*/
    QString docPath;
    QString docFile;
    format.clear();
    format<<"*.doc"<<"*.docx"<<"*.pdf"<<"*.ppt"<<"*.pptx"<<"*.xls"<<"*.xlsx"<<"*.txt";
    for(int i=0;i<format.count();i++)
    {
        foreach (docPath, docScanPath)
        {
            doctmpFile = Util::findFileList(format.at(i), docPath);
            foreach (docFile, doctmpFile)
            {
                docFileList.append(docFile);
            }
        }
    }
    /*--- end 搜索文档数据------*/

    /*---搜索视频数据------*/
    QString vidPath;
    QString vidFile;
    format.clear();
    format<<"*.mp4"<<"*.avi"<<"*.wmv"<<"*.rmvb"<<"*.rm";
    for(int i=0;i<format.count();i++)
    {
        foreach (vidPath, vidScanPath)
        {
            vidtmpFile = Util::findFileList(format.at(i), vidPath);
            foreach (vidFile, vidtmpFile)
            {
                vidFileList.append(vidFile);
            }
        }
    }

    /*下一步  设置Model...*/
    QList<TableModel*> tablelist;
    tablelist<<picModel<<docModel<<vidModel<<musModel;
    QList<QList<QString>> filelist;
    filelist<<picFileList<<docFileList<<vidFileList<<musFileList;
    QList<QCheckBox *> checklist;
    checklist<<picAllchecked<<docAllchecked<<vidAllchecked<<musAllchecked;
    for(int i=0;i<tablelist.count();i++)
    {
        QList<FileRecord> RecordFile;
        QString Record;
        int c=0;
        if (!filelist.at(i).isEmpty())
        {
            foreach (Record, filelist.at(i))
            {
                QFileInfo picRecordInfo(Record);
                FileRecord record;
                int n=0;
                for(int j=0;j<filelist.at(i).count();j++)
                {
                    QModelIndex checkIndex = tablelist.at(i)->index(j, FILE_NAME_COLUMN);
                    QVariant v=tablelist.at(i)->data(checkIndex, Qt::DisplayRole);
                    if(v.isNull())
                    {
                        break;
                    }
                    else if(v.toString()==picRecordInfo.fileName())
                    {
                        checkIndex = tablelist.at(i)->index(j, CHECK_BOX_COLUMN);
                        record.bChecked=tablelist.at(i)->data(checkIndex, Qt::UserRole).toBool();
                        if(record.bChecked)
                        {
                            c++;
                        }
                        n=1;
                        break;
                    }
                }
                if(n==0)
                {
                    record.bChecked=false;
                }
                record.strFileName = picRecordInfo.fileName();
                record.dateTime = picRecordInfo.lastModified();
                double size = (double)picRecordInfo.size()/1024;
                QString sizeDis = QString::number(size, 'f', 2);
                record.nSize = sizeDis;
                record.strFilePath = picRecordInfo.filePath();
                record.copyStart= tr("等待");
                RecordFile.append(record);
            }
            tablelist.at(i)->updateData(RecordFile);
            if(c==0)
            {
                checklist.at(i)->setCheckState(Qt::Unchecked);
            }
            else if(c==filelist.at(i).count())
            {
                checklist.at(i)->setCheckState(Qt::Checked);
            }
            else
            {
                checklist.at(i)->setCheckState(Qt::PartiallyChecked);
            }
        }
    }
}

void UserDetailWidget::updateDataModel()
{
    if(!th_doc->isRunning()&&!th_vid->isRunning()&&!th_pic->isRunning()&&!th_mus->isRunning())
        setDataAllModel();
}

/*
    QStringList picCopyFileList;
    QStringList vidCopyFileList;
    QStringList musCopyFileList;
    QStringList docCopyFileList;
    int picCopyFileCount;
    int vidCopyFileCount;
    int musCopyFileCount;
    int docCopyFileCount;

    QList < QString > picFileList;
    QList < QString > musFileList;
    QList < QString > vidFileList;
    QList < QString > docFileList;
    QCheckBox *picAllchecked;
    QCheckBox *musAllchecked;
    QCheckBox *docAllchecked;
    QCheckBox *vidAllchecked;
*/
void UserDetailWidget::getPicChecked(const QModelIndex &index)
{
    if(index.isValid())
    {
         QModelIndex checkIndex = picModel->index(index.row(), CHECK_BOX_COLUMN);
         QModelIndex fileNameIndex = picModel->index(index.row(), FILE_PATH_COLUMN);
         bool bChecked = picModel->data(checkIndex, Qt::UserRole).toBool();
         if (bChecked)
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             picCopyFileList.append(filename);
             //picCopyFileCount++;
         }
         else
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             if (picCopyFileList.contains(filename))
             {
                 picCopyFileList.removeOne(filename);
                 //picCopyFileCount--;
             }
         }
         int totalCount = picFileList.count();
         int selectCount = picCopyFileList.count();

//         qDebug() << "---------";
//         qDebug() << totalCount;
//         qDebug() << selectCount;
//         qDebug() << picCopyFileCount;
//         qDebug() << "---------";

         if(totalCount > 0 && selectCount < totalCount)
         {
             picAllchecked->setCheckState(Qt::PartiallyChecked);
         }
         if(totalCount > 0 && selectCount == totalCount)
         {
             picAllchecked->setCheckState(Qt::Checked);
         }
         if(selectCount == 0)
         {
             picAllchecked->setCheckState(Qt::Unchecked);
         }
    }
}

void UserDetailWidget::getVidChecked(const QModelIndex &index)
{

    if(index.isValid())
    {
         QModelIndex checkIndex = vidModel->index(index.row(), CHECK_BOX_COLUMN);
         QModelIndex fileNameIndex = vidModel->index(index.row(), FILE_PATH_COLUMN);
         bool bChecked = vidModel->data(checkIndex, Qt::UserRole).toBool();
         if (bChecked)
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             vidCopyFileList.append(filename);
             vidCopyFileCount++;
         }
         else
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             if (vidCopyFileList.contains(filename))
             {
                 vidCopyFileList.removeOne(filename);
                 vidCopyFileCount--;
             }
         }
         int totalCount = vidFileList.count();
         int selectCount = vidCopyFileList.count();

         if(totalCount > 0 && selectCount < totalCount)
         {
            vidAllchecked->setCheckState(Qt::PartiallyChecked);
         }
         if(totalCount > 0 && selectCount == totalCount)
         {

             vidAllchecked->setCheckState(Qt::Checked);
         }
         if(selectCount == 0)
         {

             vidAllchecked->setCheckState(Qt::Unchecked);
         }
    }
}

void UserDetailWidget::getMusicChecked(const QModelIndex &index)
{
    if(index.isValid())
    {
         QModelIndex checkIndex = musModel->index(index.row(), CHECK_BOX_COLUMN);
         QModelIndex fileNameIndex = musModel->index(index.row(), FILE_PATH_COLUMN);
         bool bChecked = musModel->data(checkIndex, Qt::UserRole).toBool();
         if (bChecked)
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             musCopyFileList.append(filename);
             musCopyFileCount++;
         }
         else
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             if (musCopyFileList.contains(filename))
             {
                 musCopyFileList.removeOne(filename);
                 musCopyFileCount--;
             }
         }
         int totalCount = musFileList.count();
         int selectCount = musCopyFileList.count();

         if(totalCount > 0 && selectCount < totalCount)
         {
             musAllchecked->setTristate(true);
             musAllchecked->setCheckState(Qt::PartiallyChecked);
         }
         if(totalCount > 0 && selectCount == totalCount)
         {
             musAllchecked->setTristate(true);
             musAllchecked->setCheckState(Qt::Checked);
         }
         if(selectCount == 0)
         {
             musAllchecked->setTristate(true);
             musAllchecked->setCheckState(Qt::Unchecked);
         }
    }
}

void UserDetailWidget::getDocChecked(const QModelIndex &index)
{
    if(index.isValid())
    {
         QModelIndex checkIndex = docModel->index(index.row(), CHECK_BOX_COLUMN);
         QModelIndex fileNameIndex = docModel->index(index.row(), FILE_PATH_COLUMN);
         bool bChecked = docModel->data(checkIndex, Qt::UserRole).toBool();
         if (bChecked)
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             docCopyFileList.append(filename);
             docCopyFileCount++;
         }
         else
         {
             QString filename = fileNameIndex.data(Qt::DisplayRole).toString();
             if (docCopyFileList.contains(filename))
             {
                 docCopyFileList.removeOne(filename);
                 docCopyFileCount--;
             }
         }
         int totalCount = docFileList.count();
         int selectCount = docCopyFileList.count();

         if(totalCount > 0 && selectCount < totalCount)
         {
             docAllchecked->setCheckState(Qt::PartiallyChecked);
         }
         if(totalCount > 0 && selectCount == totalCount)
         {
             docAllchecked->setCheckState(Qt::Checked);
         }
         if(selectCount == 0)
         {
             docAllchecked->setCheckState(Qt::Unchecked);
         }
    }
}


void UserDetailWidget::getPicCheckedSlot(const QModelIndex &index)
{
    this->getPicChecked(index);
}

void UserDetailWidget::getVidCheckedSlot(const QModelIndex &index)
{
    this->getVidChecked(index);
}
void UserDetailWidget::getMusicCheckedSlot(const QModelIndex &index)
{
    this->getMusicChecked(index);
}
void UserDetailWidget::getDocCheckedSlot(const QModelIndex &index)
{
    this->getDocChecked(index);
}

/*
    QList < QString > picFileList;
    QList < QString > musFileList;
    QList < QString > vidFileList;
    QList < QString > docFileList;

*/

void UserDetailWidget::picCheckedBoxAll(int state)
{
    if (state == Qt::Checked)
    {
        int row = picModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = picModel->index(i, CHECK_BOX_COLUMN);
                picModel->setData(checkIndex, true, Qt::UserRole);
            }
        }
        /*文件拷贝列表原样复制  picCopyFileList  picFileList*/
        QString picFile;
        picCopyFileList.clear();
        foreach(picFile, picFileList)
        {
            picCopyFileList.append(picFile);
        }
    }
    if(state == Qt::Unchecked)
    {
        int row = picModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = picModel->index(i, CHECK_BOX_COLUMN);
                picModel->setData(checkIndex, false, Qt::UserRole);
            }
        }
        /*文件拷贝列表清空，开始导出之前进行判断，如果为空则进行提示未选中文件*/
        picCopyFileList.clear();
    }
}

void UserDetailWidget::vidCheckedBoxAll(int state)
{
    if (state == Qt::Checked)
    {
        int row = vidModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = vidModel->index(i, CHECK_BOX_COLUMN);
                vidModel->setData(checkIndex, true, Qt::UserRole);
            }
        }
        /*文件拷贝列表原样复制*/
        QString vidFile;
        vidCopyFileList.clear();
        foreach(vidFile, vidFileList)
        {
            vidCopyFileList.append(vidFile);
        }
    }
    if(state == Qt::Unchecked)
    {
        int row = vidModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = vidModel->index(i, CHECK_BOX_COLUMN);
                vidModel->setData(checkIndex, false, Qt::UserRole);
            }
        }
        /*文件拷贝列表清空，开始导出之前进行判断，如果为空则进行提示未选中文件*/
        vidCopyFileList.clear();
    }

}

void UserDetailWidget::musCheckedBoxAll(int state)
{
    if (state == Qt::Checked)
    {
        int row = musModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = musModel->index(i, CHECK_BOX_COLUMN);
                musModel->setData(checkIndex, true, Qt::UserRole);
            }
        }
        /*文件拷贝列表原样复制*/
        QString musFile;
        musCopyFileList.clear();
        foreach(musFile, musFileList)
        {
            musCopyFileList.append(musFile);
        }
    }
    if(state == Qt::Unchecked)
    {
        int row = musModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = musModel->index(i, CHECK_BOX_COLUMN);
                musModel->setData(checkIndex, false, Qt::UserRole);
            }
        }
        /*文件拷贝列表清空，开始导出之前进行判断，如果为空则进行提示未选中文件*/
        musCopyFileList.clear();
    }
}

void UserDetailWidget::docCheckedBoxAll(int state)
{
    if (state == Qt::Checked)
    {
        int row = docModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = docModel->index(i, CHECK_BOX_COLUMN);
                docModel->setData(checkIndex, true, Qt::UserRole);
            }
        }
        /*文件拷贝列表原样复制*/
        QString docFile;
        docCopyFileList.clear();
        foreach(docFile, docFileList)
        {
            docCopyFileList.append(docFile);
        }
    }
    if(state == Qt::Unchecked)
    {
        int row = docModel->rowNumber();
        if (row > 0)
        {
            for (int i = 0; i < row; i++)
            {
                QModelIndex checkIndex = docModel->index(i, CHECK_BOX_COLUMN);
                docModel->setData(checkIndex, false, Qt::UserRole);
            }
        }
        /*文件拷贝列表清空，开始导出之前进行判断，如果为空则进行提示未选中文件*/
        docCopyFileList.clear();
    }
}


void UserDetailWidget::exportPicSlot()
{
    if(picCopyFileList.isEmpty())
    {
        msg_pic->setInfo(QString("数据迁移"), QString("尚未选择任何图像文件，请勾选要迁移的文件"), QPixmap(":/Message/error"), true, true);
        msg_pic->exec();
    }
    else
    {
        if(th_pic->isRunning())
        {
            msg_pic->setInfo(QString("数据迁移"),QString("图片导出正在进行"),QPixmap(":/Message/warning"),true,true);
            msg_pic->exec();
        }
        else
        {
            /*---拷贝文件---*/
            QString picDstPath = dstDirPath.append("Pictures\\");
            dstDirPath.remove("Pictures\\");
            /*创建目标目录*/
            QDir *picDstDir = new QDir();
            bool dstFlag = picDstDir->exists(picDstPath);
            if(dstFlag)
            {
                qDebug() << "目标目录已存在";
            }
            else
            {
                if(!(picDstDir->mkdir(picDstPath)))
                {
                    msg_pic->setInfo(QString("数据迁移"),QString("图片导出目录创建失败"),QPixmap(":/Message/error"),true,true);
                    msg_pic->exec();
                    delete picDstDir;
                    return;
                }
            }
            userdata_pic->CopyList=picCopyFileList;
            for(int i=0;i<picCopyFileList.count();i++)
            {
                userdata_pic->CopyToDst.insert(picCopyFileList.at(i),picDstPath);
            }
            userdata_pic->current_index=USERDATA_PIC;
            th_pic->start();
        }
    }
}

void UserDetailWidget::exportVidSlot()
{
    if(vidCopyFileList.isEmpty())
    {
        msg_vid->setInfo(QString("数据迁移"), QString("尚未选择任何视频文件，请勾选要迁移的文件"), QPixmap(":/Message/error"), true, true);
        msg_vid->exec();
    }
    else
    {
        if(th_vid->isRunning())
        {
            msg_vid->setInfo(QString("数据迁移"),QString("视频导出正在进行"),QPixmap(":/Message/warning"),true,true);
            msg_vid->exec();
        }
        else
        {
            /*---拷贝文件---*/
            QString vidDstPath = dstDirPath.append("Videos\\");
            dstDirPath.remove("Videos\\");
            /*创建目标目录*/
            QDir *vidDstDir = new QDir();
            bool dstFlag = vidDstDir->exists(vidDstPath);
            if(dstFlag)
            {
                qDebug() << "目标目录已存在";
            }
            else
            {
                if(!(vidDstDir->mkdir(vidDstPath)))
                {
                    msg_vid->setInfo(QString("数据迁移"),QString("视频导出目录创建失败"),QPixmap(":/Message/error"),true,true);
                    msg_vid->exec();
                    delete vidDstDir;
                    return;
                }
            }
            userdata_vid->CopyList=vidCopyFileList;
            for(int i=0;i<vidCopyFileList.count();i++)
            {
                userdata_vid->CopyToDst.insert(vidCopyFileList.at(i),vidDstPath);
            }
            userdata_vid->current_index=USERDATA_VID;
            th_vid->start();
        }
    }
}

void UserDetailWidget::exportMusSlot()
{
    if(musCopyFileList.isEmpty())
    {
        msg_mus->setInfo(QString("数据迁移"), QString("尚未选择任何音频文件，请勾选要迁移的文件"), QPixmap(":/Message/error"), true, true);
        msg_mus->exec();
    }
    else
    {
        if(th_mus->isRunning())
        {
            msg_mus->setInfo(QString("数据迁移"),QString("音频导出正在进行"),QPixmap(":/Message/warning"),true,true);
            msg_mus->exec();
        }
        else
        {
            /*---拷贝文件---*/
            QString musDstPath = dstDirPath.append("Musics\\");
            dstDirPath.remove("Musics\\");
            /*创建目标目录*/
            QDir *musDstDir = new QDir();
            bool dstFlag = musDstDir->exists(musDstPath);
            if(dstFlag)
            {
                qDebug() << "目标目录已存在";
            }
            else
            {
                if(!(musDstDir->mkdir(musDstPath)))
                {
                    msg_mus->setInfo(QString("数据迁移"),QString("音频导出目录创建失败"),QPixmap(":/Message/error"),true,true);
                    msg_mus->exec();
                    delete musDstDir;
                    return;
                }
            }
            userdata_mus->CopyList=musCopyFileList;
            for(int i=0;i<musCopyFileList.count();i++)
            {
                userdata_mus->CopyToDst.insert(musCopyFileList.at(i),musDstPath);
            }
            userdata_mus->current_index=USERDATA_MUS;
            th_mus->start();
        }
    }
}

void UserDetailWidget::exportDocSlot()
{
    if(docCopyFileList.isEmpty())
    {
        msg_doc->setInfo(QString("数据迁移"), QString("尚未选择任何文档，请勾选要迁移的文件"), QPixmap(":/Message/error"), true, true);
        msg_doc->exec();
    }
    else
    {
        if(th_doc->isRunning())
        {
            msg_doc->setInfo(QString("数据迁移"),QString("文档导出正在进行"),QPixmap(":/Message/warning"),true,true);
            msg_doc->exec();
        }
        else
        {
            /*---拷贝文件---*/
            QString docDstPath = dstDirPath.append("Documents\\");
            dstDirPath.remove("Documents\\");
            /*创建目标目录*/
            QDir *docDstDir = new QDir();
            bool dstFlag = docDstDir->exists(docDstPath);
            if(dstFlag)
            {
                qDebug() << "目标目录已存在";
            }
            else
            {
                if(!(docDstDir->mkdir(docDstPath)))
                {
                    msg_doc->setInfo(QString("数据迁移"),QString("文档导出目录创建失败"),QPixmap(":/Message/error"),true,true);
                    msg_doc->exec();
                    delete docDstDir;
                    return;
                }
            }
            userdata_doc->CopyList=docCopyFileList;
            for(int i=0;i<docCopyFileList.count();i++)
            {
                userdata_doc->CopyToDst.insert(docCopyFileList.at(i),docDstPath);
            }
            userdata_doc->current_index=USERDATA_DOC;
            th_doc->start();
        }
    }
}

void UserDetailWidget::reject()
{
    qDebug() << "ESC Key Event...";
}

void UserDetailWidget::UserData_copy_num_slot(int c,int num)
{
    QString n;
    QList<CustomMessageBox*> l;
    l<<msg_doc<<msg_pic<<msg_mus<<msg_vid;
    int i;
    switch(c)
    {
    case USERDATA_DOC:
        th_doc->exit();
        n=tr("文档数据导出");
        i=0;
        break;
    case USERDATA_MUS:
        th_mus->exit();
        n=tr("音频数据导出");
        i=2;
        break;
    case USERDATA_PIC:
        th_pic->exit();
        n=tr("图片数据导出");
        i=1;
        break;
    case USERDATA_VID:
        th_vid->exit();
        n=tr("视频数据导出");
        i=3;
        break;
    }
    if(!num)
    {

        l.at(i)->setInfo(QString("数据迁移"),n+QString("成功，是否对数据文件进行压缩？"),QPixmap(":/Message/yes"),true, false);
        l.at(i)->exec();

    }
    else
    {
        l.at(i)->setInfo(QString("数据迁移"),n+QString("失败:")+QString::number(num)+QString("个"),QPixmap(":/Message/warning"),true,true);
        l.at(i)->exec();
    }
}

void UserDetailWidget::UserData_num_Slot(int c,double num)
{
    if(num<=0)
    {
        picProgressBar->hide();
        musProgressBar->hide();
        vidProgressBar->hide();
        docProgressBar->hide();
    }
    else
    {
        switch(c)
        {
        case 0://图片
            picProgressBar->show();
            picProgressBar->setValue(num);
            break;
        case 1://视频
            vidProgressBar->show();
            vidProgressBar->setValue(num);
            break;
        case 2://音乐
            musProgressBar->show();
            musProgressBar->setValue(num);
            break;
        case 3://文档
            docProgressBar->show();
            docProgressBar->setValue(num);
            break;
        }
    }
}

void UserDetailWidget::UserData_over_name_slot(int c,QString name)
{
    updataTable(c,name,tr("导出完成"));
}

void UserDetailWidget::UserData_error_name_Slot(int c,QString name)
{
    updataTable(c,name,tr("导出失败"));
}

void UserDetailWidget::UserData_current_copy_name_slot(int c,QString name)
{
    updataTable(c,name,tr("正在导出"));
}

void UserDetailWidget::updataTable(int c,QString name,QString stat)
{
    QList<TableModel*> tablelist;
    tablelist<<picModel<<docModel<<vidModel<<musModel;
    QList<QList<QString>> filelist;
    filelist<<picFileList<<docFileList<<vidFileList<<musFileList;
    QList<QCheckBox *> checklist;
    checklist<<picAllchecked<<docAllchecked<<vidAllchecked<<musAllchecked;
    int i=0;
    switch(c)
    {
    case USERDATA_PIC://图片
        i=0;
        break;
    case USERDATA_VID://视频
        i=2;
        break;
    case USERDATA_MUS://音乐
        i=3;
        break;
    case USERDATA_DOC://文档
        i=1;
        break;
    }
    QList<FileRecord> RecordFile;
    QString Record;
    int c=0;
    if (!filelist.at(i).isEmpty())
    {
        foreach (Record, filelist.at(i))
        {
            QFileInfo picRecordInfo(Record);
            FileRecord record;
            int n=0;
            for(int j=0;j<filelist.at(i).count();j++)
            {
                QModelIndex checkIndex = tablelist.at(i)->index(j, FILE_NAME_COLUMN);
                QVariant v=tablelist.at(i)->data(checkIndex, Qt::DisplayRole);
                if(v.isNull())
                {
                    break;
                }
                else if(v.toString()==picRecordInfo.fileName())
                {
                    if(v.toString()==name)
                    {
                        record.copyStart=stat;
                    }
                    else
                    {
                        checkIndex=tablelist.at(i)->index(j,FILE_COPY_COLUMN);
                        record.copyStart=tablelist.at(i)->data(checkIndex,Qt::DisplayRole).toString();
                    }
                    checkIndex = tablelist.at(i)->index(j, CHECK_BOX_COLUMN);
                    record.bChecked=tablelist.at(i)->data(checkIndex, Qt::UserRole).toBool();
                    if(record.bChecked)
                    {
                        c++;
                    }
                    n=1;
                    break;
                }
            }
            if(n==0)
            {
                record.bChecked=false;
            }
            record.strFileName = picRecordInfo.fileName();
            record.dateTime = picRecordInfo.lastModified();
            double size = (double)picRecordInfo.size()/1024;
            QString sizeDis = QString::number(size, 'f', 2);
            record.nSize = sizeDis;
            record.strFilePath = picRecordInfo.filePath();
            RecordFile.append(record);
        }
        tablelist.at(i)->updateData(RecordFile);
        if(c==0)
        {
            checklist.at(i)->setCheckState(Qt::Unchecked);
        }
        else if(c==filelist.at(i).count())
        {
            checklist.at(i)->setCheckState(Qt::Checked);
        }
        else
        {
            checklist.at(i)->setCheckState(Qt::PartiallyChecked);
        }
    }
}

void UserDetailWidget::userData_oneKey()
{
    userData_count=0;
    userData_over=0;
    updateDataModel();
    picCheckedBoxAll(Qt::Checked);
    musCheckedBoxAll(Qt::Checked);
    docCheckedBoxAll(Qt::Checked);
    vidCheckedBoxAll(Qt::Checked);

    QList<QString> dirPathlist;
    dirPathlist<<"Pictures"<<"Musics"<<"Documents"<<"Videos";
    QList<QString> dstpath;
    QDir *DstDir = new QDir();
    int x=0;
    for(int i=0;i<dirPathlist.count();i++)
    {
        dstpath.append(dstDirPath.append(dirPathlist.at(i)+"\\"));
        bool dstFlag = DstDir->exists(dstDirPath);
        if(dstFlag)
        {
            qDebug() << "目标目录已存在";
        }
        else
        {
            if(!(DstDir->mkdir(dstDirPath)))
            {
                x++;
            }
        }
        dstDirPath.remove(dirPathlist.at(i)+"\\");
    }
    delete DstDir;
    if(x>0)
    {
        msg_doc->setInfo(QString("数据迁移"),QString::number(x)+QString("个目录创建失败"),QPixmap(":/Message/error"),true,true);
        msg_doc->exec();
        return;
    }
    userdata_one->CopyList=picCopyFileList+musCopyFileList+docCopyFileList+vidCopyFileList;
    userData_count=userdata_one->CopyList.count();
    QList<QStringList> lcopy;
    lcopy<<picCopyFileList<<musCopyFileList<<docCopyFileList<<vidCopyFileList;
    for(int i=0;i<lcopy.count();i++)
    {
        for(int j=0;j<lcopy.at(i).count();j++)
        {
            userdata_one->CopyToDst.insert(lcopy.at(i).at(j),dstpath[i]);
        }
    }
}

void UserDetailWidget::userData_oneKey_start()
{
    th_one->start();
}

void UserDetailWidget::UserData_one_overSlot(int c, int name)
{
    th_one->exit();
    emit userData_oneKey_over(c,name);
}

void UserDetailWidget::UserData_progress_num_slot(int c, int cout, int over)
{
    userData_over=over;
    emit userData_oneKey_progress_num(cout,over);
}

void UserDetailWidget::compressData()
{
    QString destination = Util::readDstFileFromIni();
    QString dstCompressPack = destination.append(".zip");
    destination.remove(".zip");
    JlCompress::compressDir(dstCompressPack, destination);
}
