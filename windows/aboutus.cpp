#include "windows/aboutus.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPen>


AboutUsDialog::AboutUsDialog(QWidget *parent)
    :QDialog(parent)
{
    resize(520, 280);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowIcon(QPixmap(":/title/logo"));

    titleIcon = new QLabel();
    titleLabel = new QLabel();
    closeButton = new QPushButton();
    ensureButton = new QPushButton();

    titleLabel->setStyleSheet("QLabel{color:white;}");
    titleIcon->setPixmap(QPixmap(":/title/logo"));
    titleIcon->setFixedSize(16, 16);
    titleIcon->setScaledContents(true);
    closeButton->setFixedSize(18, 18);
    closeButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/close);}"
                                 "QPushButton:hover{border-radius:5px; border-image:url(:/title/dclose);}"
                                 "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dclose);}");
    closeButton->setContentsMargins(0, 0, 0, 5);
    ensureButton->setFixedSize(75, 25);
    ensureButton->setStyleSheet("QPushButton{border:1px solid lightgray; background:rgb(230,230,230);}"
                           "QPushButton:hover{border-color:green; background:transparent;}");
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIcon, 0, Qt::AlignVCenter);
    titleLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton, 0, Qt::AlignVCenter);
    titleLayout->setSpacing(5);
    titleLayout->setContentsMargins(2, 0, 2, 0);

    titleInfo = new QLabel();
    infoLabel = new QLabel();
    versionLabel = new QLabel();
    authorLabel = new QLabel();
    copyrightLabel = new QLabel();
    iconLabel = new QLabel();

    titleInfo->setStyleSheet("QLabel{color:rgb(43,165,237); font: bold; font-size:28px; font-family:黑体;}");
    infoLabel->setStyleSheet("QLabel{color:rgb(43,165,237); font-size:15px; font-family:黑体;}");
    versionLabel->setStyleSheet("QLabel{color:rgb(43,165,237);}");
    copyrightLabel->setStyleSheet("QLabel{color:gray;}");

    QPixmap iconPixmap(":/title/blogo");
    iconLabel->setPixmap(iconPixmap);
    iconLabel->setFixedSize(iconPixmap.size());


    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(titleInfo);
    leftLayout->addWidget(infoLabel);
    leftLayout->addSpacing(18);
    leftLayout->addWidget(versionLabel);
    //leftLayout->addWidget(authorLabel);
    leftLayout->addWidget(copyrightLabel);
    leftLayout->setSpacing(3);
    leftLayout->setContentsMargins(0, 30, 0, 30);

    QHBoxLayout *centerHLayout = new QHBoxLayout();
    centerHLayout->addLayout(leftLayout);
    centerHLayout->addWidget(iconLabel);
    centerHLayout->setSpacing(0);
    centerHLayout->setContentsMargins(40, 5, 20, 10);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(ensureButton);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(0, 0, 30, 20);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(titleLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(centerHLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(8, 8, 8, 0);
    setLayout(mainLayout);

    connect(ensureButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    translateLanguage();
}

AboutUsDialog::~AboutUsDialog()
{
}

void AboutUsDialog::translateLanguage()
{

}

void AboutUsDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        mouse_press = true;
    }
    move_point = event->globalPos() - pos();
}

void AboutUsDialog::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}

void AboutUsDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_press)
    {
        QPoint move_pos = event->globalPos();
        move(move_pos - move_point);
    }
}

void AboutUsDialog::paintEvent(QPaintEvent *)
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
