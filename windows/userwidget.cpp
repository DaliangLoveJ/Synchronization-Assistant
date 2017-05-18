#include "windows/userwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QStackedWidget>
#include <QListWidget>
#include <QListWidgetItem>

UserWidget::UserWidget(QWidget *parent)
    :QWidget(parent)
{

    QVBoxLayout *mainVlayout = new QVBoxLayout();
    initUserTopWidget();
    mainVlayout->addWidget(topWidget);
    topWidget->setContentsMargins(0, 0, 0, 0);
    mainVlayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainVlayout);
    setContentsMargins(0, 0, 0,0);
}

UserWidget::~UserWidget()
{

}

void UserWidget::initUserTopWidget()
{
    topWidget = new QWidget();
    pictureButton = new QPushButton();
    musicButton = new QPushButton();
    documentButton = new QPushButton();
    videoButton = new QPushButton();

    pictureButton->setFixedSize(339, 160);
    musicButton->setFixedSize(339, 160);
    documentButton->setFixedSize(339, 160);
    videoButton->setFixedSize(339, 160);
    pictureButton->setStyleSheet( "QPushButton{border-radius:5px; border-image:url(:/userwin/userpic);}"
                                  "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/duserpic);}"
                                  "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/duserpic);}");
    musicButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/usermusic);}"
                             "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/dusermusic);}"
                             "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/dusermusic);}");
    documentButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/userdoc);}"
                             "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/duserdoc);}"
                             "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/duserdoc);;}");
    videoButton->setStyleSheet("QPushButton{border-radius:5px; border-image:url(:/userwin/uservideo);;}"
                             "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/duservideo);}"
                             "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/duservideo);}");
    QHBoxLayout *hlayout1 = new QHBoxLayout();
    QHBoxLayout *hlayout2 = new QHBoxLayout();
    QVBoxLayout *Vlayout = new QVBoxLayout();
    hlayout1->addWidget(pictureButton);
    hlayout1->addWidget(musicButton);
    hlayout1->setSpacing(1);
    hlayout1->setContentsMargins(110, 0, 110, 0);

    hlayout2->addWidget(videoButton);
    hlayout2->addWidget(documentButton);
    hlayout2->setSpacing(1);
    hlayout2->setContentsMargins(110, 0, 110, 0);
    Vlayout->addLayout(hlayout1);
    Vlayout->addLayout(hlayout2);
    Vlayout->setSpacing(1);
    Vlayout->setContentsMargins(10, 75, 10, 75);
    topWidget->setLayout(Vlayout);
}



