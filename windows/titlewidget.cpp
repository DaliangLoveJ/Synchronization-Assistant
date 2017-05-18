#include "windows/titlewidget.h"
#include <QLabel>
#include <QPushButton>
#include <QPoint>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>


TitleWidget::TitleWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initWidget();
    this->translateLanguage();
    connect(login_button, SIGNAL(clicked()), SIGNAL(showLogin()));
    connect(setup_button, SIGNAL(clicked()), SIGNAL(showMainMenu()));
    connect(min_button, SIGNAL(clicked()), SIGNAL(showMin()));
    connect(close_button, SIGNAL(clicked()), SIGNAL(closeWidget()));

    computer->setCheckable(true);
    computer->setChecked(true);
    move_is = false;
}

TitleWidget::~TitleWidget()
{
}

void TitleWidget::mousePressEvent(QMouseEvent *e)
{
    press_point = e->pos();
    move_is = true;
}

void TitleWidget::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons()&Qt::LeftButton) && move_is)
    {
        static QWidget *parWidget = this->parentWidget();
        QPoint parPoint = parWidget->pos();
        parPoint.setX(parPoint.x() + e->x() - press_point.x());
        parPoint.setY(parPoint.y() + e->y() - press_point.y());
        parWidget->move(parPoint);
    }
}

void TitleWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(move_is)
    {
        move_is = false;
    }
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    emit showMax();
}

void TitleWidget::initWidget()
{
    login_button = new QPushButton();
    setup_button = new QPushButton();
    min_button = new QPushButton();
    max_button = new QPushButton();
    close_button = new QPushButton();;
    title_text = new QLabel();
    title_icon = new QLabel();
    title_layout = new QHBoxLayout();
    computer = new ToolButton(":/title/computer");
    user = new ToolButton(":/title/user");
    application = new ToolButton(":/title/application");
    system = new ToolButton(":/title/configure");


    computer->setStyleSheet("QToolButton:hover{border-radius:5px; border-image:url(:/title/backtb);}"
                            "QToolButton:checked{border-radius:5px; border-image:url(:/title/backtb);}"
                            "QToolButton:pressed{border-radius:5px; border-image:url(:/title/backtb);}");

    user->setStyleSheet("QToolButton:hover{border-radius:5px; border-image:url(:/title/backtb);}"
                        "QToolButton:checked{border-radius:5px; border-image:url(:/title/backtb);}"
                        "QToolButton:pressed{border-radius:5px; border-image:url(:/title/backtb);}");
    application->setStyleSheet("QToolButton:hover{border-radius:5px; border-image:url(:/title/backtb);}"
                                "QToolButton:checked{border-radius:5px; border-image:url(:/title/backtb);}"
                                "QToolButton:pressed{border-radius:5px; border-image:url(:/title/backtb);}");
    system->setStyleSheet("QToolButton:hover{border-radius:5px; border-image:url(:/title/backtb);}"
                            "QToolButton:checked{border-radius:5px; border-image:url(:/title/backtb);}"
                            "QToolButton:pressed{border-radius:5px; border-image:url(:/title/backtb);}");

    logo = new QLabel();
    dataText = new QLabel();
    logo->setPixmap(QPixmap(":/title/logo"));

    dataText->setContentsMargins(QMargins(0, 0, 0, 5));
    dataText->setStyleSheet("QLabel{color:white; font-size:28px; font-family:微软雅黑;}");
    title_text->setStyleSheet("QLabel{color:white; font-family:微软雅黑;}");
    title_text->setContentsMargins(QMargins(0, 0, 0, 0));


    login_button->setFixedSize(18, 18);
    setup_button->setFixedSize(18, 18);
    min_button->setFixedSize(18, 18);
    close_button->setFixedSize(18, 18);
    login_button->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/login);}"
                                "QPushButton:hover{border-radius:5px; border-image:url(:/title/dlogin);}"
                                "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dlogin);}");

    setup_button->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/setup);}"
                                "QPushButton:hover{border-radius:5px; border-image:url(:/title/dsetup);}"
                                "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dsetup);}");
    min_button->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/min);}"
                                "QPushButton:hover{border-radius:5px; border-image:url(:/title/dmin);}"
                                "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dmin);}");
    close_button->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/title/close);}"
                                "QPushButton:hover{border-radius:5px; border-image:url(:/title/dclose);}"
                                "QPushButton:pressed{border-radius:5px; border-image:url(:/title/dclose);}");

    title_layout->addStretch();
    title_layout->addWidget(title_text, 0, Qt::AlignTop);
    title_layout->addWidget(login_button, 0, Qt::AlignTop);
    title_layout->addWidget(setup_button, 0, Qt::AlignTop);
    title_layout->addWidget(min_button, 0, Qt::AlignTop);
    title_layout->addWidget(close_button, 0, Qt::AlignTop);
    title_layout->setSpacing(8);
    QVBoxLayout *main_layout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(computer);
    buttonLayout->addWidget(user);
    buttonLayout->addWidget(application);
    buttonLayout->addWidget(system);
    buttonLayout->addStretch();
    buttonLayout->addWidget(logo);
    buttonLayout->addWidget(dataText);
    buttonLayout->setSpacing(8);
    buttonLayout->setContentsMargins(0, 5, 0, 0);

    main_layout->addLayout(title_layout);
    main_layout->addLayout(buttonLayout);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);

    setLayout(main_layout);
    setFixedHeight(109);
}

void TitleWidget::translateLanguage()
{
    title_text->setText(tr("欢迎使用系统同步助手"));
    computer->setText(tr("我的电脑"));
    user->setText(tr("用户"));
    application->setText(tr("应用"));
    system->setText(tr("系统"));
    dataText->setText(tr("同步助手"));
}


