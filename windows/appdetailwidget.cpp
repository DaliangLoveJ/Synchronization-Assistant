#include "windows/appdetailwidget.h"
#include "windows/comboboxdelegate.h"
#include "windows/customdelegate.h"
#include "windows/messagebox.h"
#include "functions/emaildata.h"
#include "functions/browserdata.h"
#include "functions/common.h"
#include "tools/utils.h"
#include "JlCompress.h"
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
#include <QCheckBox>
#include <QComboBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QFileDialog>
#include <QProgressBar>
#include <QProcess>
#include <QSettings>
#include <QCoreApplication>


AppDetailWidget::AppDetailWidget(QWidget *parent)
    :QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    dstFilePath = Util::readDstFileFromIni();
    dstFilePath.append("\\");
    emailTreeView = new QTreeView();
    emailTreeView->setContentsMargins(0, 0, 0, 0);
    emailTreeView->setStyleSheet("QTreeView{border:0px;}"
                                 "QTreeView::item{font-size:30px; font-family:黑体; height:26px;}"
                                 "QTreeView::item:hover{color:black; background:transparent; font-size:30px; font-family:黑体; height:26px;}"
                                 "QTreeView::item:selected{color:black; background:transparent; font-size:30px; font-family:黑体; height:26px;}");
    emailTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    emailModel = new QStandardItemModel(emailTreeView);
    emailTreeView->setModel(emailModel);
    customDelegate = new CustomDelegate();
    emailTreeView->setMouseTracking(true);
    emailTreeView->setItemDelegate(customDelegate);
    outlookItem = new QStandardItem(QIcon(":/appwin/outlook"),QStringLiteral("Microsoft Outlook"));
    foxmailItem = new QStandardItem(QIcon(":/appwin/foxmail"),QStringLiteral("Foxmail 7"));
    ComboBoxDelegate  *ieComboxDelegate = new ComboBoxDelegate(this);
    ComboBoxDelegate  *firefoxComboxDelegate = new ComboBoxDelegate(this);
    ComboBoxDelegate  *chromeComboxDelegate = new ComboBoxDelegate(this);
    browser = new BrowserData();
    connect(browser,SIGNAL(BrowserData_error_name(int,int,int)),this,SLOT(browserData_error_name_slot(int,int,int)));
    connect(browser,SIGNAL(BrowserData_progress_num(double)),this,SLOT(browserData_progress_num_slot(double)));
    browser_one = new BrowserData();
    connect(browser_one,SIGNAL(BrowserData_one_progress_num(int,int)),this,SLOT(browserData_one_progress_num_slot(int,int)));
    email = new EmailData();
    email_one = new EmailData();
    ieBtn = new QPushButton();
    firfoxBtn = new QPushButton();
    chromeBtn = new QPushButton();
    IEcombox = new QComboBox();    
    firfoxCombox = new QComboBox();    
    chromeCombox = new QComboBox();
    IEcombox->setItemDelegate(ieComboxDelegate);
    firfoxCombox->setItemDelegate(firefoxComboxDelegate);
    chromeCombox->setItemDelegate(chromeComboxDelegate);

    connect(ieComboxDelegate, SIGNAL(deleteItem(QModelIndex)),this, SLOT(iecomboboxdelete(QModelIndex)));
    connect(firefoxComboxDelegate, SIGNAL(deleteItem(QModelIndex)),this, SLOT(firefoxcomboboxdelete(QModelIndex)));
    connect(chromeComboxDelegate, SIGNAL(deleteItem(QModelIndex)),this, SLOT(chromecomboboxdelete(QModelIndex)));
    connect(email,SIGNAL(emailData_error_num(int,int)),this,SLOT(emailData_error_num_slot(int,int)));
    connect(email,SIGNAL(emailData_progress_num(double)),this,SLOT(emailData_current_num_slot(double)));
    connect(email_one,SIGNAL(emailData_one_progress_num(int,int)),this,SLOT(emailData_one_progress_num_slot(int,int)));

    initAppEmailWidget();
    initAppBrowerWidget();
    updateBrowserData();
    checkClient();
    appLeftListWidget = new QListWidget();
    appRightStackWidget = new QStackedWidget();
    QListWidgetItem *emailDetalItem = new QListWidgetItem(tr("邮件"));
    QListWidgetItem *browserDetailItem = new QListWidgetItem(tr("收藏夹"));
    emailDetalItem->setTextAlignment(Qt::AlignCenter);
    browserDetailItem->setTextAlignment(Qt::AlignCenter);
    appLeftListWidget->setFixedWidth(97);
    appLeftListWidget->insertItem(0, emailDetalItem);
    appLeftListWidget->insertItem(1, browserDetailItem);
    appLeftListWidget->setStyleSheet("QListWidget{border:0px;background-color:rgb(244, 244, 244)}"
                                     "QListWidget::item{color:rgb(101, 101, 101); height: 40px;}"
                                     "QListWidget::item:hover{color:white; background-color:rgb(51, 198, 235);}"
                                     "QListWidget::item:selected{border:0px; color:white; background-color:rgb(51, 168, 235);}");
    appLeftListWidget->setFocusPolicy(Qt::NoFocus);
    appLeftListWidget->setCurrentRow(0);
    appRightStackWidget->insertWidget(0, emailDetalWidget);
    appRightStackWidget->insertWidget(1, browserDetailWidget);
    QHBoxLayout *centerHlayout = new QHBoxLayout();
    centerHlayout->addWidget(appLeftListWidget, 0, Qt::AlignLeft);
    centerHlayout->addWidget(appRightStackWidget);
    centerHlayout->setSpacing(0);
    centerHlayout->setContentsMargins(0, 0, 0, 1);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(centerHlayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    msg = new CustomMessageBox();
    connect(appLeftListWidget, SIGNAL(currentRowChanged(int)), appRightStackWidget,SLOT(setCurrentIndex(int)));
    connect(appLeftListWidget, SIGNAL(currentRowChanged(int)), this,SLOT(updateEmailDataSlot()));
    connect(customDelegate,SIGNAL(open(QModelIndex)),this,SLOT(emailBtnSlot(QModelIndex)));
    connect(emailModel, SIGNAL(itemChanged(QStandardItem*)),this,SLOT(treeItemChanged(QStandardItem*)));
    connect(emailModel, SIGNAL(itemChanged(QStandardItem*)),this,SLOT(getemailItemData(QStandardItem*)));
    connect(ieBtn, SIGNAL(clicked()),this,SLOT(IEBrowserAdd()));
    connect(firfoxBtn, SIGNAL(clicked()),this,SLOT(FirefoxBrowserAdd()));
    connect(chromeBtn, SIGNAL(clicked()),this,SLOT(ChromeBrowserAdd()));
    connect(msg, SIGNAL(ensureBtnClicked()), this, SLOT(compressData())) ;
}

AppDetailWidget::~AppDetailWidget()
{
}


void AppDetailWidget::iecomboboxdelete(const QModelIndex &index)
{
    IEcombox->removeItem(index.row());
}

void AppDetailWidget::firefoxcomboboxdelete(const QModelIndex &index)
{
    firfoxCombox->removeItem(index.row());
}

void AppDetailWidget::chromecomboboxdelete(const QModelIndex &index)
{
    chromeCombox->removeItem(index.row());
}

void AppDetailWidget::paintEvent(QPaintEvent *)
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


void AppDetailWidget::translateLanguage()
{
}


void AppDetailWidget::initAppEmailWidget()
{
    emailDetalWidget = new QWidget();
    QStandardItem *emailItem = new QStandardItem(QStringLiteral("邮件客户端数据"));
    //emailItem
    emailModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral(""));
    emailModel->appendRow(emailItem);
    emailItem->appendRow(outlookItem);
    emailItem->appendRow(foxmailItem);

    emailProgressBar = new QProgressBar();
    emailProgressBar->setStyleSheet("QProgressBar{border:none; color:white; text-align:center; background:white; }"
                                    "QProgressBar::chunk{background: rgb(0, 160, 230); }");


    QLabel *emailCheckPrompt = new QLabel();
    QPushButton *emailStartExport = new QPushButton();
    emailCheckPrompt->setStyleSheet("QLabel{color:gray;}");
    emailCheckPrompt->setText(tr("检测到以下数据可以选择导出:"));
    emailStartExport->setFixedSize(90, 28);
    emailStartExport->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/dstartexport); color:gray;}"
                                    "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/startexport); color:gray;}"
                                    "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/ddstartexport); color:gray;}");
    browserInfo = new QLabel();
    /*设置一下QSS 在检测的过程中设置text*/
    browserInfo->setStyleSheet("QLabel{color:lightgray;}");
    emailInfo = new QLabel();
    emailInfo->setStyleSheet("QLabel{color:lightgray;}");
    //browserInfo->setText(tr("Microsoft Outlook 未安装"));



    QCheckBox *emailAllchecked = new QCheckBox();
    emailAllchecked->setText(tr("全选"));
    emailExportToPath = new QPushButton();
    QLabel *emailExportToLabel = new QLabel();
    emailAllchecked->setStyleSheet("QCheckBox{color:gray;}");
    emailExportToLabel->setStyleSheet("QLabel{color:gray;}");
    emailExportToPath->setStyleSheet("QPushButton{border:none; color:rgb(51, 168, 235); background:white;}");
    emailExportToLabel->setText(tr("导出到"));
    emailExportToPath->setText(this->dstFilePath);

    QVBoxLayout *emailMainLayout = new QVBoxLayout();
    QHBoxLayout *emailTopMainLayout = new QHBoxLayout();
    QVBoxLayout *emailCenterMainLayout = new QVBoxLayout();
    QHBoxLayout *emailBottomMainLayout = new QHBoxLayout();

    emailTopMainLayout->addWidget(emailCheckPrompt);
    emailTopMainLayout->addSpacing(10);
    emailTopMainLayout->addWidget(emailProgressBar);
    emailTopMainLayout->addSpacing(10);
    emailTopMainLayout->addWidget(emailStartExport);
    emailTopMainLayout->setContentsMargins(10, 12, 10, 0);
    emailCenterMainLayout->addWidget(emailTreeView);
    emailCenterMainLayout->setContentsMargins(0, 0, 0, 0);
    emailCenterMainLayout->setMargin(0);
    emailBottomMainLayout->addWidget(emailInfo);
    emailBottomMainLayout->addSpacing(80);
    emailBottomMainLayout->addWidget(emailExportToLabel);
    emailBottomMainLayout->addWidget(emailExportToPath);
    emailBottomMainLayout->addStretch();
    emailBottomMainLayout->setContentsMargins(12, 0, 0, 5);
    emailBottomMainLayout->setSpacing(4);
    emailMainLayout->setMargin(0);
    emailMainLayout->addLayout(emailTopMainLayout);
    emailMainLayout->addLayout(emailCenterMainLayout);
    emailMainLayout->addLayout(emailBottomMainLayout);
    emailMainLayout->setSpacing(0);
    emailMainLayout->setContentsMargins(0, 0, 1, 0);
    emailDetalWidget->setLayout(emailMainLayout);
    connect(emailStartExport, SIGNAL(clicked(bool)),this, SLOT(exportEmailDataSlot()));

}

void AppDetailWidget::initAppBrowerWidget()
{
    browserDetailWidget = new QWidget();
    QLabel *browserCheckPrompt = new QLabel();
    QPushButton *browserStartExport = new QPushButton();
    browserCheckPrompt->setStyleSheet("QLabel{color:gray;}");
    browserCheckPrompt->setText(tr("检测到以下数据可以选择导出:"));
    browserStartExport->setFixedSize(90, 28);
    browserStartExport->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/dstartexport); color:gray;}"
                                      "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/startexport); color:gray;}"
                                      "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/ddstartexport); color:gray;}");


    browserProgressBar = new QProgressBar();
    browserProgressBar->setStyleSheet("QProgressBar{border:none; color:white; text-align:center; background:white; }"
                                  "QProgressBar::chunk{background: rgb(0, 160, 230); }");

    QCheckBox *browserAllchecked = new QCheckBox();
    browserAllchecked->setText(tr("全选"));
    browserExportToPath = new QPushButton();
    QLabel *browserExportToLabel = new QLabel();
    browserAllchecked->setStyleSheet("QCheckBox{color:gray;}");
    browserExportToLabel->setStyleSheet("QLabel{color:gray;}");
    browserExportToPath->setStyleSheet("QPushButton{border:none; color:rgb(51, 168, 235); background:white;}");
    browserExportToLabel->setText(tr("导出到"));
    browserExportToPath->setText(this->dstFilePath);

    QCheckBox *IESel = new QCheckBox();
    QLabel    *IEIcon = new QLabel();
    QLabel    *IETitle = new QLabel();

    IEcombox->setEditable(true);
    IEcombox->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:550px; background-color:white;}"
                            "QComboBox QAbstractItemView::item{height:22px;}"
                            "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                            "QComboBox::drop-down{border:0px;}");
    ieBtn->setStyleSheet("QPushButton{border-image:url(:/icon/file);}"
                         "QPushButton:hover{border-image:url(:/icon/dfile);}"
                         "QPushButton:pressed{border-image:url(:/icon/dfile);}");
    ieBtn->setFixedSize(26, 20);
    ieBtn->setContentsMargins(0, 0, 0, 1);

    /*是否要显示多行数据，下拉列表*/
    IETitle->setStyleSheet("QLabel{color:gray;}");
    IEIcon->setPixmap(QPixmap(":/appwin/ie"));
    IETitle->setText(tr("IE浏览器收藏夹"));
    QHBoxLayout *IEHlayout = new QHBoxLayout();
    IEHlayout->addSpacing(12);
    IEHlayout->addWidget(IESel);
    IEHlayout->addSpacing(15);
    IEHlayout->addWidget(IEIcon);
    IEHlayout->addWidget(IETitle);
    IEHlayout->addSpacing(30);
    IEHlayout->addWidget(IEcombox);
    IEHlayout->addWidget(ieBtn);
    IEHlayout->setSpacing(5);
    IEHlayout->addStretch();

    QCheckBox *firefoxSel = new QCheckBox();
    QLabel    *firefoxIcon = new QLabel();
    QLabel    *firefoxTitle = new QLabel();

    firfoxCombox->setEditable(true);
    firfoxCombox->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:550px; background-color:white;}"
                            "QComboBox QAbstractItemView::item{height:22px;}"
                            "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                            "QComboBox::drop-down{border:0px;}");
    firfoxBtn->setStyleSheet("QPushButton{border-image:url(:/icon/file);}"
                         "QPushButton:hover{border-image:url(:/icon/dfile);}"
                         "QPushButton:pressed{border-image:url(:/icon/dfile);}");
    firfoxBtn->setFixedSize(26, 20);
    firfoxBtn->setContentsMargins(0, 0, 0, 1);

    /*是否要显示多行数据，下拉列表*/
    firefoxTitle->setStyleSheet("QLabel{color:gray;}");
    firefoxIcon->setPixmap(QPixmap(":/appwin/firefox"));
    firefoxTitle->setText(tr("火狐浏览器收藏夹"));
    QHBoxLayout *firefoxHlayout = new QHBoxLayout();
    firefoxHlayout->addSpacing(12);
    firefoxHlayout->addWidget(firefoxSel);
    firefoxHlayout->addSpacing(15);
    firefoxHlayout->addWidget(firefoxIcon);
    firefoxHlayout->addWidget(firefoxTitle);
    firefoxHlayout->addSpacing(18);
    firefoxHlayout->addWidget(firfoxCombox);
    firefoxHlayout->addWidget(firfoxBtn);
    firefoxHlayout->setSpacing(5);
    firefoxHlayout->addStretch();

    QCheckBox *chromeSel = new QCheckBox();
    QLabel    *chromeIcon = new QLabel();
    QLabel    *chromeTitle = new QLabel();

    chromeCombox->setEditable(true);
    chromeCombox->setStyleSheet("QComboBox{border:0.5px solid lightgray; height:18px; width:550px; background-color:white;}"
                            "QComboBox QAbstractItemView::item{height:22px;}"
                            "QComboBox::down-arrow{image:url(:/icon/arrow);}"
                            "QComboBox::drop-down{border:0px;}");
    chromeBtn->setStyleSheet("QPushButton{border-image:url(:/icon/file);}"
                         "QPushButton:hover{border-image:url(:/icon/dfile);}"
                         "QPushButton:pressed{border-image:url(:/icon/dfile);}");
    chromeBtn->setFixedSize(26, 20);
    chromeBtn->setContentsMargins(0, 0, 0, 1);

    /*是否要显示多行数据，下拉列表*/
    chromeTitle->setStyleSheet("QLabel{color:gray;}");
    chromeIcon->setPixmap(QPixmap(":/appwin/chrome"));
    chromeTitle->setText(tr("谷歌浏览器收藏夹"));
    QHBoxLayout *chromeHlayout = new QHBoxLayout();
    chromeHlayout->addSpacing(12);
    chromeHlayout->addWidget(chromeSel);
    chromeHlayout->addSpacing(15);
    chromeHlayout->addWidget(chromeIcon);
    chromeHlayout->addWidget(chromeTitle);
    chromeHlayout->addSpacing(18);
    chromeHlayout->addWidget(chromeCombox);
    chromeHlayout->addWidget(chromeBtn);
    chromeHlayout->setSpacing(5);
    chromeHlayout->addStretch();

    QVBoxLayout *browserMainLayout = new QVBoxLayout();
    QHBoxLayout *browserTopMainLayout = new QHBoxLayout();
    QVBoxLayout *browserCenterMainLayout = new QVBoxLayout();
    QHBoxLayout *browserBottomMainLayout = new QHBoxLayout();
    browserTopMainLayout->addWidget(browserCheckPrompt);
    browserTopMainLayout->addSpacing(10);
    browserTopMainLayout->addWidget(browserProgressBar);
    browserTopMainLayout->addSpacing(10);
    browserTopMainLayout->addWidget(browserStartExport);
    browserTopMainLayout->setContentsMargins(10, 20, 10, 5);
    browserCenterMainLayout->addLayout(IEHlayout);
    browserCenterMainLayout->addLayout(firefoxHlayout);
    browserCenterMainLayout->addLayout(chromeHlayout);
    browserCenterMainLayout->setSpacing(15);
    browserCenterMainLayout->addStretch();
    browserCenterMainLayout->setContentsMargins(0, 15, 0, 0);
    browserBottomMainLayout->addWidget(browserInfo);

    browserBottomMainLayout->addSpacing(80);
    browserBottomMainLayout->addWidget(browserExportToLabel);
    browserBottomMainLayout->addWidget(browserExportToPath);
    browserBottomMainLayout->addStretch();
    browserBottomMainLayout->setContentsMargins(12, 0, 0, 5);
    browserBottomMainLayout->setSpacing(4);
    browserMainLayout->setMargin(0);
    browserMainLayout->addLayout(browserTopMainLayout);
    browserMainLayout->addLayout(browserCenterMainLayout);
    browserMainLayout->addLayout(browserBottomMainLayout);
    browserMainLayout->setSpacing(0);
    browserMainLayout->setContentsMargins(0, 0, 1, 0);
    browserDetailWidget->setLayout(browserMainLayout);

    connect(IESel, SIGNAL(stateChanged(int)), this, SLOT(IECheckBoxSlot(int)));
    connect(firefoxSel, SIGNAL(stateChanged(int)), this, SLOT(FireFoxCheckBoxSlot(int)));
    connect(chromeSel, SIGNAL(stateChanged(int)), this, SLOT(ChromeCheckBoxSlot(int)));
    connect(browserStartExport, SIGNAL(clicked()), this, SLOT(exportBrowserDataSlot()));
}


void AppDetailWidget::updateBrowserData()
{
    QString chromePath;
    QString firefoxPath;

    IEDir = browser->IEDir;
    chromeFile = browser->chromeFile;
    firefoxFile = browser->firefoxFile;
    if(!IEDir.isEmpty())
    {
        int i=0;
        for(i=0;i<IEcombox->count();i++)
        {
            if(IEcombox->itemText(i)==IEDir)
            {
                break;
            }
        }
        if(IEcombox->itemText(i)!=IEDir)
            IEcombox->addItem(IEDir);
    }

    if(!chromeFile.isEmpty())
    {
        foreach(chromePath, chromeFile)
        {
            int i=0;
            for(int i=0;i<chromeCombox->count();i++)
            {
                if(chromeCombox->itemText(i)==chromePath)
                {
                    break;
                }
            }
            if(chromeCombox->itemText(i)!=chromePath)
                chromeCombox->addItem(chromePath);
        }
    }

    if(!firefoxFile.isEmpty())
    {
        foreach(firefoxPath, firefoxFile)
        {
            int i=0;
            for(int i=0;i<firfoxCombox->count();i++)
            {
                if(firfoxCombox->itemText(i)==firefoxPath)
                {
                    break;
                }
            }
            if(firfoxCombox->itemText(i)!=firefoxPath)
                firfoxCombox->addItem(firefoxPath);
        }
    }

    if(IEDir.isEmpty())
    {
        browserInfo->setText(tr("IE浏览器未安装"));
    }
    if (firefoxFile.isEmpty())
    {
        browserInfo->setText(tr("火狐浏览器未安装"));
    }
    if(chromeFile.isEmpty())
    {
        browserInfo->setText(tr("谷歌浏览器未安装"));
    }

    if(IEDir.isEmpty() && firefoxFile.isEmpty())
    {
        browserInfo->setText(tr("IE浏览器 火狐浏览器未安装"));
    }
    if (IEDir.isEmpty() && chromeFile.isEmpty())
    {
        browserInfo->setText(tr("IE浏览器 谷歌浏览器未安装"));
    }
    if (firefoxFile.isEmpty() && chromeFile.isEmpty())
    {
        browserInfo->setText(tr("火狐浏览器 谷歌浏览器未安装"));
    }
    if(IEDir.isEmpty() && firefoxFile.isEmpty() && chromeFile.isEmpty())
    {
        browserInfo->setText(tr("IE浏览器 谷歌浏览器 火狐浏览器未安装"));
    }
}

void AppDetailWidget::updateBrowserSlot()
{
   updateBrowserData();
}

bool AppDetailWidget::updateFoxmail()
{
    bool ret=false;
    QDir *dir=new QDir("c:/");
    QFileInfoList list=dir->entryInfoList();
    QString currenttime=QDateTime::currentDateTime().toString("yyyyMMdd");
    QFileInfo emaillist;
    for(int i=0;i<list.count();i++)
    {
        if(list.at(i).fileName().contains(currenttime))
        {
            emaillist=list.at(i);
            ret=true;
            break;
        }
    }
    if(!ret)
    {
        return ret;
    }
    dir->setPath(emaillist.filePath());
    foxmailFileList=Util::findFileList("*.eml",emaillist.filePath());
    list.clear();
    list=dir->entryInfoList();
    for(int i=0;i<list.count();i++)
    {
        QString str;
        QString accountName=list.at(i).fileName();
        if(accountName==".."||accountName==".")
        {
            continue;
        }
        for (int i=0; i < foxmailItem->rowCount();++i)
        {
            str = foxmailItem->index().child(i, 0).data().toString();
            if (str==accountName)
            {
                break;
            }
        }
        if(str!=accountName)
        {
            QStandardItem *accountItem = new QStandardItem(QIcon(":/appwin/account"), accountName);
            accountItem->setCheckable(true);
            foxmailItem->appendRow(accountItem);
        }
    }
    delete dir;
    return ret;
}

void AppDetailWidget::updateEmailData()
{
    QStringList outlookAccount;
    QString     accountName;
    emailTreeView->expandAll();
   /*outlook data update...*/
    if (email->checkOutlook())
    {
        outlookItem->setCheckable(true);
        outlookItem->setTristate(true);
        /*添加并更新outlook账户列表*/
        outlookAccount = email->getOutlookAccount();
        if(!outlookAccount.isEmpty())
        {
            foreach (accountName, outlookAccount)
            {
                QString str;
                for (int i=0; i < outlookItem->rowCount();++i)
                {
                    str = outlookItem->index().child(i, 0).data().toString();
                    if (str==accountName)
                    {
                        break;
                    }
                }
                if(str!=accountName)
                {
                    QStandardItem *accountItem = new QStandardItem(QIcon(":/appwin/account"), accountName);
                    accountItem->setCheckable(true);
                    outlookItem->appendRow(accountItem);
                }
            }
        }
    }
    else
    {
        outlookItem->setCheckable(false);
        outlookItem->setCheckState(Qt::PartiallyChecked);
        emailInfo->setText(tr("Microsoft Outlook邮件客户端未安装"));
    }

    /*Foxmail data update...*/
    if(email->checkFoxmail())
    {
        foxmailItem->setCheckable(true);
        foxmailItem->setTristate(true);
        updateFoxmail();
    }
    else
    {
        foxmailItem->setCheckable(false);
        foxmailItem->setCheckState(Qt::PartiallyChecked);
        emailInfo->setText(tr("Foxmail 客户端未安装"));
    }
    if(!email->checkOutlook() && !email->checkFoxmail())
    {
        emailInfo->setText(tr("Microsoft Outlook客户端未安装 Foxmail客户端未安装"));
    }
    connect(emailTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(getEmailTreeView(QModelIndex)));
}

void AppDetailWidget::updateEmailDataSlot()
{
    updateEmailData();
    emailProgressBar->hide();
    browserProgressBar->hide();
}


void AppDetailWidget::emailBtnSlot(const QModelIndex &index)
{
    QString data = index.data().toString();
    if (data == "Microsoft Outlook")
    {
        QString path=QFileDialog::getOpenFileName(this,tr("添加PST文件"),"c:/","*.pst");
        if(!path.isEmpty())
        {
            QFileInfo info;
            info.setFile(path);
            QStandardItem *accountItem = new QStandardItem(QIcon(":/appwin/account"), info.fileName()+"  ("+info.filePath()+")");
            accountItem->setCheckable(true);
            outlookItem->appendRow(accountItem);
            if(outlookItem->rowCount()==1)
            {
                outlookItem->setCheckState(Qt::Unchecked);
            }
            else
            {
                if(outlookItem->checkState()==Qt::Checked)
                {
                    outlookItem->setCheckState(Qt::PartiallyChecked);
                }
            }
        }
    }
    if(data == "Foxmail 7")
    {
//        QString path;
//        QDir dir;
//        path=dir.currentPath();
        QProcess *autoguiFoxmail = new QProcess();
        msg->setInfo(QString("Foxmail 7"), QString("Foxmail 7邮件导出准备开始，请先关闭杀毒软件，准备完成后退出此界面！！！"), QPixmap(":/Message/question"), true, true);
        msg->exec();
        autoguiFoxmail->start("./plugins/Foxmail7/FoxmailMigration.exe");
        if(!autoguiFoxmail->waitForStarted())
        {
            msg->setInfo(QString("Foxmail 7"), QString("FoxmailMigration.exe启动失败"), QPixmap(":/Message/error"), true, true);
            msg->exec();
        }
        if(!autoguiFoxmail->waitForFinished())
        {
        }
        if(!updateFoxmail())
        {
            msg->setInfo(QString("Foxmail 7"), QString("Foxmail导出失败"), QPixmap(":/Message/error"), true, true);
            msg->exec();
            return ;
        }
        else
        {
            msg->setInfo(QString("Foxmail 7"), QString("Foxmail导出完成，是否对数据文件进行压缩？"), QPixmap(":/Message/yes"), true, false);
            msg->exec();
            foxmailItem->setCheckState(Qt::Unchecked);
            return ;
        }
    }
}


void AppDetailWidget::treeItemChanged(QStandardItem * item)
{
    if(item == nullptr)
            return;
    if(item->isCheckable())
    {
        //如果条目是存在复选框的，那么就进行下面的操作
        Qt::CheckState state = item->checkState();//获取当前的选择状态
        if(item->isTristate())
        {
            //如果条目是三态的，说明可以对子目录进行全选和全不选的设置
            if(state != Qt::PartiallyChecked)
            {
                //当前是选中状态，需要对其子项目进行全选
                treeItem_checkAllChild(item,state == Qt::Checked ? true : false);
            }
        }
        else
        {
            //说明是两态的，两态会对父级的三态有影响
            //判断兄弟节点的情况
            treeItem_CheckChildChanged(item);
        }
    }
}

void AppDetailWidget::treeItem_checkAllChild(QStandardItem * item,bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0; i < rowCount; ++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

void AppDetailWidget::treeItem_checkAllChild_recursion(QStandardItem * item, bool check )
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

Qt::CheckState AppDetailWidget::checkSibling(QStandardItem * item)
{
    //先通过父节点获取兄弟节点
    QStandardItem * parent = item->parent();
    if(nullptr == parent)
       return item->checkState();

    int brotherCount = parent->rowCount();
    int checkedCount(0),unCheckedCount(0);
    Qt::CheckState state;
    for(int i=0;i<brotherCount;++i)
    {
       QStandardItem* siblingItem = parent->child(i);
       state = siblingItem->checkState();
       if(Qt::PartiallyChecked == state)
           return Qt::PartiallyChecked;
       else if(Qt::Unchecked == state)
           ++unCheckedCount;
       else
           ++checkedCount;
       if(checkedCount>0 && unCheckedCount>0)
           return Qt::PartiallyChecked;
    }
    if(unCheckedCount>0)
       return Qt::Unchecked;
    return Qt::Checked;
}

void AppDetailWidget::treeItem_CheckChildChanged(QStandardItem * item)
{
    if(nullptr == item)
            return;
    Qt::CheckState siblingState = checkSibling(item);
    QStandardItem * parentItem = item->parent();
    if(nullptr == parentItem)
        return;
    if(Qt::PartiallyChecked == siblingState)
    {
        if(parentItem->isCheckable() && parentItem->isTristate())
            parentItem->setCheckState(Qt::PartiallyChecked);
    }
    else if(Qt::Checked == siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItem_CheckChildChanged(parentItem);
}


void AppDetailWidget::IEBrowserAdd()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("添加浏览器收藏夹文件"), "C:");
    if(!fileName.isEmpty())
    {
        int i=0;
        for(i=0;i<IEcombox->count();i++)
        {
            if(IEcombox->itemText(i)==fileName)
            {
                break;
            }
        }
        if(IEcombox->itemText(i)!=fileName)
            IEcombox->addItem(fileName);
    }
}

void AppDetailWidget::FirefoxBrowserAdd()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("添加浏览器收藏夹文件"), "C:");
    if(!fileName.isEmpty())
    {
        firfoxCombox->addItem(fileName);
    }
}

void AppDetailWidget::ChromeBrowserAdd()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("添加浏览器收藏夹文件"), "C:");
    if(!fileName.isEmpty())
    {
        chromeCombox->addItem(fileName);
    }
}

void AppDetailWidget::checkClient()
{
    //调用其BrowserData   EmailData类中的客户端检测函数，进行本地显示
    //browserInfo->setText(tr("Firefox浏览器未安装"));
    //emailInfo->setText(tr("Microsoft Outlook客户端未安装"));
 }

void AppDetailWidget::IECheckBoxSlot(int state)
{
   IECopyFileList.clear();
   if(state == Qt::Checked)
   {
       int count = IEcombox->count();
       for (int i = 0;i < count; i++)
       {
           IEcombox->setCurrentIndex(i);
           QString currentText = IEcombox->currentText();
           IECopyFileList.append(currentText);
       }
   }
   if(state == Qt::Unchecked)
   {
       IECopyFileList.clear();
   }
}

void AppDetailWidget::ChromeCheckBoxSlot(int state)
{
    ChromeCopyFileList.clear();
    if(state == Qt::Checked)
    {
        int count = chromeCombox->count();
        for (int i = 0;i < count; i++)
        {
            chromeCombox->setCurrentIndex(i);
            QString currentText = chromeCombox->currentText();
            ChromeCopyFileList.append(currentText);
        }
    }

    if(state == Qt::Unchecked)
    {
        ChromeCopyFileList.clear();
    }
}

void AppDetailWidget::FireFoxCheckBoxSlot(int state)
{
    FireFoxCopyFileList.clear();
    if(state == Qt::Checked)
    {
        int count = firfoxCombox->count();
        for (int i = 0;i < count; i++)
        {
            firfoxCombox->setCurrentIndex(i);
            QString currentText = firfoxCombox->currentText();
            FireFoxCopyFileList.append(currentText);
        }
    }
    if(state == Qt::Unchecked)
    {
        FireFoxCopyFileList.clear();
    }
}


void AppDetailWidget::exportBrowserDataSlot()
{
    if(IECopyFileList.isEmpty() && ChromeCopyFileList.isEmpty() && FireFoxCopyFileList.isEmpty())
    {
        msg->setInfo(QString("数据迁移"), QString("尚未选择浏览器收藏夹文件"), QPixmap(":/Message/error"), true, true);
        msg->exec();
    }
    else
    {
        if(browser->isRunning())
        {
            msg->setInfo(QString("数据迁移"), QString("正在进行收藏夹数据导出"), QPixmap(":/Message/warning"), true, true);
            msg->exec();
        }
        else
        {
            if(!IECopyFileList.isEmpty())
            {

                QString IEDstPath = dstFilePath.append("Browser\\IE\\");
                dstFilePath.remove("Browser\\IE\\");

                bool IEDstDirFlag;
                QDir *IEDstDir = new QDir();
                bool dstFlag = IEDstDir->exists(IEDstPath);
                if(dstFlag)
                {
                    qDebug() << "目标目录已存在";
                }
                else
                {
                    IEDstDirFlag = IEDstDir->mkpath(IEDstPath);
                }
                browser->IEcopylist.clear();
                browser->IEDstPathlist.clear();
                for(int i=0;i<IECopyFileList.count();i++)
                {
                    QFileInfoList l=Util::convenienceDir(IECopyFileList.at(i));
                    for(int j=0;j<l.count();j++)
                    {
                        browser->IEcopylist.append(l.at(j).filePath());
                        QString f=IECopyFileList.at(i);
                        int lf=l.at(j).filePath().length()-f.length();
                        f=l.at(j).filePath().right(lf);
                        int n=f.length()-l.at(j).fileName().length();
                        browser->IEDstPathlist.append(IEDstPath+f.left(n));
                    }
                }
            }
            if(!ChromeCopyFileList.isEmpty())
            {
                QString chromeDstPath = dstFilePath.append("Browser\\Chrome\\");
                dstFilePath.remove("Browser\\Chrome\\");
                bool chromeDstDirFlag;
                QDir *chromeDstDir = new QDir();
                bool dstFlag = chromeDstDir->exists(chromeDstPath);
                if(dstFlag)
                {
                    qDebug() << "目标目录已存在";
                }
                else
                {
                    chromeDstDirFlag = chromeDstDir->mkpath(chromeDstPath);
                }
                browser->Chromecopylist.clear();
                for(int i=0;i<ChromeCopyFileList.count();i++)
                {
                    QFileInfo l(ChromeCopyFileList.at(i));
                    browser->Chromecopylist.append(l.filePath());
                }
                browser->ChromeDstPath=chromeDstPath;
            }

            if(!FireFoxCopyFileList.isEmpty())
            {
                QString firefoxDstPath = dstFilePath.append("Browser\\Firefox\\");
                dstFilePath.remove("Browser\\Firefox\\");

                bool firefoxDstDirFlag;
                QDir *firefoxDstDir = new QDir();
                bool dstFlag = firefoxDstDir->exists(firefoxDstPath);
                if(dstFlag)
                {
                    qDebug() << "目标目录已存在";
                }
                else
                {
                    firefoxDstDirFlag = firefoxDstDir->mkpath(firefoxDstPath);
                }
                browser->Firefoxcopylist.clear();
                for(int i=0;i<FireFoxCopyFileList.count();i++)
                {
                    QFileInfo l(FireFoxCopyFileList.at(i));
                    browser->Firefoxcopylist.append(l.filePath());
                }
                browser->FirefoxDstPath=firefoxDstPath;
            }
            browser->start();
        }
    }
}

void AppDetailWidget::browserData_error_name_slot(int ie,int ch,int fir)
{
    if(ie==0&&ch==0&&fir==0)
    {
        msg->setInfo(QString("数据迁移"), QString("收藏夹导出完成，是都对数据文件进行压缩？"), QPixmap(":/Message/yes"), true, false);
        msg->exec();
        return;
    }
    QString n;
    if(ie!=0)
    {
        n="IE ";
    }
    if(ch!=0)
    {
        n+="Chrome ";
    }
    if(fir!=0)
    {
        n+="Firefox ";
    }
    msg->setInfo(QString("数据迁移"), n+QString("导出失败"), QPixmap(":/Message/error"), true, true);
    msg->exec();
}

void AppDetailWidget::browserData_progress_num_slot(double num)
{
    if(num<0)
    {
        browserProgressBar->hide();
    }
    else
    {
        browserProgressBar->show();
        browserProgressBar->setValue(num);
    }
}

void AppDetailWidget::getEmailTreeView(const QModelIndex &index)
{

}

void AppDetailWidget::getemailItemData(QStandardItem * item)
{
    if(item == nullptr)
        return;
    if(item->isCheckable())
    {
        Qt::CheckState state = item->checkState();//获取当前的选择状态
        if(state == Qt::Checked||state==Qt::PartiallyChecked)
        {
           QString data = item->index().data().toString();
           if(!emailFileList.contains(data) && !data.isEmpty())
           {
               emailFileList.append(data);
           }
        }
        else if(state == Qt::Unchecked)
        {
            QString data = item->index().data().toString();
            if(emailFileList.contains(data))
            {
                emailFileList.removeOne(data);
            }
        }
    }
}

void AppDetailWidget::exportEmailDataSlot()
{
    if(email->isRunning())
    {
        msg->setInfo(QString("数据迁移"), QString("邮件正在导出"), QPixmap(":/Message/error"), true, true);
        msg->exec();
    }
    else
    {
        /*创建邮件目标目录*/
        QString outlookDstFilePath = dstFilePath.append("Email\\Outlook\\");
        dstFilePath.remove("Email\\Outlook\\");
        QString foxmailDstFilePath = dstFilePath.append("Email\\Foxmail\\");
        dstFilePath.remove("Email\\Foxmail\\");
        bool emailDstDirFlag;
        QDir *emailDstDir = new QDir();
        bool dstFlag = emailDstDir->exists(outlookDstFilePath);
        if(dstFlag)
        {
            qDebug() << "目标目录已存在";
        }
        else
        {
            emailDstDirFlag = emailDstDir->mkpath(outlookDstFilePath);
        }
        dstFlag=emailDstDir->exists(foxmailDstFilePath);
        if(dstFlag)
        {
            qDebug() << "目标目录已存在";
        }
        else
        {
            emailDstDirFlag = emailDstDir->mkpath(foxmailDstFilePath);
        }
        QList <QString> outlookFilePath;
        outlookFilePath = email->findOutlookPst();
        if(outlookFilePath.isEmpty())
        {
            qDebug()<<"Outlook NULL";
            return;
        }

        if(emailFileList.isEmpty())
        {
            msg->setInfo(QString("数据迁移"), QString("尚未选择邮件数据文件或未安装任何邮件客户端"), QPixmap(":/Message/error"), true, true);
            msg->exec();
            return;
        }
        QList<QString> other;
        for(int i=0;i<outlookItem->rowCount();i++)
        {
            QString s=outlookItem->index().child(i,0).data().toString();
            if((outlookItem->child(i,0)->checkState()==Qt::Checked)&&(s.contains(" (")))
            {
                int l=s.section(" (",1,1).count()-1;
                other.append(s.section(" (",1,1).left(l));
            }
        }
        if(emailFileList.contains("Microsoft Outlook"))
        {
            email->outlookScanPath.clear();
            for(int i=0;i<outlookFilePath.count();i++)
            {
                QFileInfo info(outlookFilePath.at(i));
                if(emailFileList.contains(info.fileName()))
                {
                    email->outlookScanPath.append(outlookFilePath.at(i));
                }
            }
            email->outlookToPath=outlookDstFilePath;
            email->outlookotherScanPath=other;
        }
        if(emailFileList.contains("Foxmail 7"))
        {
            email->foxmailScanPath.clear();
            for(int i=0;i<foxmailFileList.count();i++)
            {
                QFileInfo info(foxmailFileList.at(i));
                if(emailFileList.contains(info.fileName()))
                {
                    email->foxmailScanPath.append(foxmailFileList.at(i));
                }
            }
            email->foxmailToPath=foxmailDstFilePath;
        }
        email->start();
    }
}


void AppDetailWidget::emailData_error_num_slot(int out, int fox)
{
    if(!emailFileList.contains("Microsoft Outlook"))
    {
        out=1;
    }
    if(!emailFileList.contains("Foxmail 7"))
    {
        fox=1;
    }
    if(out&&fox)
    {
        msg->setInfo(QString("数据迁移"), QString("邮件数据文件导出成功，是否对数据文件进行压缩？"), QPixmap(":/Message/yes"), true, false);
        msg->exec();
    }
    else if((!out)&&(!fox))
    {
        msg->setInfo(QString("数据迁移"), QString("邮件数据文件导出失败"), QPixmap(":/Message/error"), true, true);
        msg->exec();
    }
    else if(!out)
    {
        msg->setInfo(QString("数据迁移"), QString("Outlook数据文件导出失败"), QPixmap(":/Message/error"), true, true);
        msg->exec();
    }
    else
    {
        msg->setInfo(QString("数据迁移"), QString("Foxmail数据文件导出失败"), QPixmap(":/Message/error"), true, true);
        msg->exec();
    }
}


void AppDetailWidget::emailData_current_num_slot(double num)
{
    if(num<0)
    {
        emailProgressBar->hide();
    }
    else
    {
        emailProgressBar->show();
        emailProgressBar->setValue(num);
    }
}


void AppDetailWidget::reject()
{
    qDebug() << "App ESC Key Event...";
}

void AppDetailWidget::updateReadIniFilePath()
{
    QSettings *appSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                            QCoreApplication::organizationName(),
                                            QCoreApplication::applicationName());
    appSetting->beginGroup(DESTINATION_GROUNP);
    dstFilePath = appSetting->value(DEFAULT).toString();
    emailExportToPath->setText(dstFilePath);
    browserExportToPath->setText(dstFilePath);
    appSetting->endGroup();
    delete appSetting;

    dstFilePath.append("\\");
    bool   mkDstDirFlag;
    QDir *dstDir = new QDir();
    bool dstExist = dstDir->exists(dstFilePath);
    if (dstExist)
    {
        qDebug() << "user dst dir has been created...";
    }
    else
    {
        mkDstDirFlag = dstDir->mkdir(dstFilePath);
        if(mkDstDirFlag)
        {
            qDebug() << "user dst dir created failed...";
        }
    }
}

void AppDetailWidget::appData_oneKey()
{
    if(email->checkFoxmail())
    {
//        QProcess *autoguiFoxmail = new QProcess();
//        msg->setInfo(QString("Foxmail 7"), QString("Foxmail 7邮件导出准备开始，请先关闭杀毒软件，准备完成后退出此界面！！！"), QPixmap(":/Message/question"), true, true);
//        msg->exec();
//        autoguiFoxmail->start("./plugins/Foxmail7/FoxmailMigration.exe");
//        if(!autoguiFoxmail->waitForStarted())
//        {
//            msg->setInfo(QString("Foxmail 7"), QString("FoxmailMigration.exe启动失败"), QPixmap(":/Message/error"), true, true);
//            msg->exec();
//        }
//        if(!autoguiFoxmail->waitForFinished())
//        {
//        }
//        if(!updateFoxmail())
//        {
//            msg->setInfo(QString("Foxmail 7"), QString("Foxmail导出失败"), QPixmap(":/Message/error"), true, true);
//            msg->exec();
//            return ;
//        }
//        foxmailItem->setCheckState(Qt::Unchecked);
    }
    app_one_email_over=0;
    app_one_browser_over=0;
    updateEmailDataSlot();
    outlookItem->setCheckState(Qt::Checked);
    foxmailItem->setCheckState(Qt::Checked);
    updateBrowserSlot();
    IECheckBoxSlot(Qt::Checked);
    ChromeCheckBoxSlot(Qt::Checked);
    FireFoxCheckBoxSlot(Qt::Checked);
    QString outlookDstFilePath = dstFilePath.append("Email\\Outlook\\");
    dstFilePath.remove("Email\\Outlook\\");
    QString foxmailDstFilePath = dstFilePath.append("Email\\Foxmail\\");
    dstFilePath.remove("Email\\Foxmail\\");
    bool emailDstDirFlag;
    QDir *emailDstDir = new QDir();
    bool dstFlag = emailDstDir->exists(outlookDstFilePath);
    if(dstFlag)
    {
        qDebug() << "目标目录已存在";
    }
    else
    {
        emailDstDirFlag = emailDstDir->mkpath(outlookDstFilePath);
    }
    dstFlag=emailDstDir->exists(foxmailDstFilePath);
    if(dstFlag)
    {
        qDebug() << "目标目录已存在";
    }
    else
    {
        emailDstDirFlag = emailDstDir->mkpath(foxmailDstFilePath);
    }
    QList <QString> outlookFilePath;
    outlookFilePath = email->findOutlookPst();
    if(outlookFilePath.isEmpty())
    {
        qDebug()<<"Outlook NULL";
        return;
    }
    if(emailFileList.isEmpty())
    {
        msg->setInfo(QString("数据迁移"), QString("尚未选择邮件数据文件或未安装任何邮件客户端"), QPixmap(":/Message/error"), true, true);
        msg->exec();
        return;
    }
    QList<QString> other;
    for(int i=0;i<outlookItem->rowCount();i++)
    {
        QString s=outlookItem->index().child(i,0).data().toString();
        if((outlookItem->child(i,0)->checkState()==Qt::Checked)&&(s.contains(" (")))
        {
            int l=s.section(" (",1,1).count()-1;
            other.append(s.section(" (",1,1).left(l));
        }
    }
    if(emailFileList.contains("Microsoft Outlook"))
    {
        email_one->outlookScanPath.clear();
        for(int i=0;i<outlookFilePath.count();i++)
        {
            QFileInfo info(outlookFilePath.at(i));
            if(emailFileList.contains(info.fileName()))
            {
                email_one->outlookScanPath.append(outlookFilePath.at(i));
            }
        }
        email_one->outlookToPath=outlookDstFilePath;
        email_one->outlookotherScanPath=other;
    }
    if(emailFileList.contains("Foxmail 7"))
    {
        email_one->foxmailScanPath.clear();
        for(int i=0;i<foxmailFileList.count();i++)
        {
            QFileInfo info(foxmailFileList.at(i));
            if(emailFileList.contains(info.fileName()))
            {
                email_one->foxmailScanPath.append(foxmailFileList.at(i));
            }
        }
        email_one->foxmailToPath=foxmailDstFilePath;
    }

    if(!IECopyFileList.isEmpty())
    {

        QString IEDstPath = dstFilePath.append("Browser\\IE\\");
        dstFilePath.remove("Browser\\IE\\");

        bool IEDstDirFlag;
        QDir *IEDstDir = new QDir();
        bool dstFlag = IEDstDir->exists(IEDstPath);
        if(dstFlag)
        {
            qDebug() << "目标目录已存在";
        }
        else
        {
            IEDstDirFlag = IEDstDir->mkpath(IEDstPath);
        }
        browser_one->IEcopylist.clear();
        browser_one->IEDstPathlist.clear();
        for(int i=0;i<IECopyFileList.count();i++)
        {
            QFileInfoList l=Util::convenienceDir(IECopyFileList.at(i));
            for(int j=0;j<l.count();j++)
            {
                browser_one->IEcopylist.append(l.at(j).filePath());
                QString f=IECopyFileList.at(i);
                int lf=l.at(j).filePath().length()-f.length();
                f=l.at(j).filePath().right(lf);
                int n=f.length()-l.at(j).fileName().length();
                browser_one->IEDstPathlist.append(IEDstPath+f.left(n));
            }
        }
    }

    if(!ChromeCopyFileList.isEmpty())
    {
        QString chromeDstPath = dstFilePath.append("Browser\\Chrome\\");
        dstFilePath.remove("Browser\\Chrome\\");
        bool chromeDstDirFlag;
        QDir *chromeDstDir = new QDir();
        bool dstFlag = chromeDstDir->exists(chromeDstPath);
        if(dstFlag)
        {
            qDebug() << "目标目录已存在";
        }
        else
        {
            chromeDstDirFlag = chromeDstDir->mkpath(chromeDstPath);
        }
        browser_one->Chromecopylist.clear();
        for(int i=0;i<ChromeCopyFileList.count();i++)
        {
            QFileInfo l(ChromeCopyFileList.at(i));
            browser_one->Chromecopylist.append(l.filePath());
        }
        browser_one->ChromeDstPath=chromeDstPath;
    }

    if(!FireFoxCopyFileList.isEmpty())
    {
        QString firefoxDstPath = dstFilePath.append("Browser\\Firefox\\");
        dstFilePath.remove("Browser\\Firefox\\");

        bool firefoxDstDirFlag;
        QDir *firefoxDstDir = new QDir();
        bool dstFlag = firefoxDstDir->exists(firefoxDstPath);
        if(dstFlag)
        {
            qDebug() << "目标目录已存在";
        }
        else
        {
            firefoxDstDirFlag = firefoxDstDir->mkpath(firefoxDstPath);
        }
        browser_one->Firefoxcopylist.clear();
        for(int i=0;i<FireFoxCopyFileList.count();i++)
        {
            QFileInfo l(FireFoxCopyFileList.at(i));
            browser_one->Firefoxcopylist.append(l.filePath());
        }
        browser_one->FirefoxDstPath=firefoxDstPath;
    }
    app_one_count=email_one->outlookScanPath.count()+email_one->outlookotherScanPath.count()+email_one->foxmailScanPath.count()
            +browser_one->IEcopylist.count()+browser_one->Chromecopylist.count()+browser_one->Firefoxcopylist.count();
}

void AppDetailWidget::appData_oneKey_start()
{
    email_one->start();
    browser_one->start();
}

void AppDetailWidget::browserData_one_progress_num_slot(int coun, int over)
{
    app_one_browser_over=over;
    emit appData_onekey_progress_num(app_one_count,app_one_browser_over);
}

void AppDetailWidget::emailData_one_progress_num_slot(int coun, int over)
{
    app_one_email_over=over;
    emit appData_onekey_progress_num(app_one_count,app_one_email_over);
}

void AppDetailWidget::compressData()
{
    QString destination = Util::readDstFileFromIni();
    QString dstCompressPack = destination.append(".zip");
    destination.remove(".zip");
    JlCompress::compressDir(dstCompressPack, destination);
}
