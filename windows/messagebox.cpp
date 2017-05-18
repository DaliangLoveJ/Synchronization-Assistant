#include <windows/messagebox.h>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

CustomMessageBox::CustomMessageBox(QWidget *parent)
    : QDialog(parent)
{
    this->resize(400, 160);
    int width = this->width();
    int height = this->height();
    //初始化为未按下鼠标左键
    mouse_press = false;
    //设置标题栏隐藏
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    closeButton = new QPushButton(this);
    closeButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/close);}"
                                "QPushButton:hover{border-radius:5px; border-image:url(:/title/dclose);}"
                                "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dclose);}");
    closeButton->setGeometry(width - 22, 4, 18, 18);
    titleLabel = new QLabel(this);
    titleLabel->setObjectName(QString::fromUtf8("labelOne"));
    titleLabel->setStyleSheet("QLabel{color:white;}");
    titleLabel->setGeometry(0, 0, width-50, 30);
    msgLabel = new QLabel(this);
    msgLabel->setGeometry(20, 50, 36, 36);
    msgLabel->setScaledContents(true);

    askLabel = new QLabel(this);
    askLabel->setGeometry(65, 60, width-100, 25*2);
    askLabel->setWordWrap(true);
    askLabel->setAlignment(Qt::AlignTop);

    checkBox = new QCheckBox(this);
    checkBox->setGeometry(10, height - 35, 160, 25);
    checkBox->setHidden(true);
    cancelButton = new QPushButton(this);
    cancelButton->resize(70, 25);
    cancelButton->move(width - cancelButton->width() - 10, height - 35);
    ensueButton = new QPushButton(this);
    ensueButton->resize(70, 25);
    ensueButton->move(width - ensueButton->width() - cancelButton->width() - 20, height - 35);
    checkBox->setStyleSheet("background:transparent;");
    ensueButton->setObjectName(QString::fromUtf8("pushButtonTwo"));
    cancelButton->setObjectName(QString::fromUtf8("pushButtonTwo"));


    QObject::connect(ensueButton, SIGNAL(clicked()), this, SLOT(cancelOperate()));
    QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(cancelOperate()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelOperate()));
    QObject::connect(ensueButton, SIGNAL(clicked()), this, SIGNAL(ensureBtnClicked()));
    this->translateLanguage();
}

CustomMessageBox::~CustomMessageBox()
{
}


void CustomMessageBox::translateLanguage()
{
    ensureText = tr("确定");
    cancelText = tr("取消");
}
void CustomMessageBox::setInfo(QString title_info, QString info, QPixmap pixmap, bool is_check_hidden, bool is_ok_hidden)
{
    titleLabel->setText(QString("  ") + title_info);
    //设置提示信息
    askLabel->setText(info);
    msgLabel->setPixmap(pixmap);
    //是否隐藏复选框
    checkBox->setChecked(false);
    checkBox->setHidden(is_check_hidden);
    //是否隐藏确定按钮
    ensueButton->setHidden(is_ok_hidden);
    if(is_ok_hidden)
    {
        cancelButton->setText(ensureText);
    }
    else
    {
        ensueButton->setText(ensureText);
        cancelButton->setText(cancelText);
    }
    //设置默认按钮为取消按钮
    cancelButton->setFocus();
}

void CustomMessageBox::ensureOperate()
{
    bool is_hidden = checkBox->isHidden();
    if(!is_hidden)
    {
      bool is_checked = checkBox->isChecked();
      emit msgChecked(is_checked, true);
    }
    this->accept();
}

void CustomMessageBox::cancelOperate()
{
    bool is_check_hidden = checkBox->isHidden();
    bool is_ok_hidden = ensueButton->isHidden();
    if(!is_check_hidden)
    {
        bool is_checked = checkBox->isChecked();
        if(!is_ok_hidden)
        {
            emit msgChecked(is_checked, false);
        }
        else
        {
            emit ensureMessageHidden(is_checked);
        }
    }
    this->reject();
}

void CustomMessageBox::paintEvent(QPaintEvent *)
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
    painterWidget.drawRect(QRect(0, 26, this->width(), this->height()-26));

    QPainter painterFrame(this);
    painterFrame.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 26), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 26)};
    painterFrame.drawPolyline(points, 4);
}
void CustomMessageBox::mousePressEvent(QMouseEvent * event )
{
    //只能是鼠标左键移动和改变大小
    if(event->buttons() == Qt::LeftButton)
    {
        mouse_press = true;
    }
    //窗口移动距离
    move_point = event->globalPos() - pos();
}
void CustomMessageBox::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}
void CustomMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    if(mouse_press)
    {
      QPoint move_pos = event->globalPos();
      move(move_pos - move_point);
    }
}


