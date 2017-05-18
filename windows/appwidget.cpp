#include "windows/appwidget.h"
#include <QPushButton>
#include <QHBoxLayout>

AppWidget::AppWidget(QWidget *parent)
    :QWidget(parent)
{

    browseButton = new QPushButton();
    emailButton = new QPushButton();

    browseButton->setFixedSize(339, 160);
    emailButton->setFixedSize(339, 160);

    emailButton->setStyleSheet( "QPushButton{border-radius:5px; border-image:url(:/userwin/email);}"
                              "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/demail);}"
                              "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/demail);}");
    browseButton->setStyleSheet( "QPushButton{border-radius:5px; border-image:url(:/userwin/browser);}"
                              "QPushButton:hover{border-radius:5px; border-image:url(:/userwin/dbrowser);}"
                              "QPushButton:pressed{border-radius:5px; border-image:url(:/userwin/dbrowser);}");

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(emailButton);
    hlayout->addWidget(browseButton);
    hlayout->setSpacing(1);
    hlayout->setContentsMargins(120, 100, 120, 140);
    setLayout(hlayout);
}

AppWidget::~AppWidget()
{

}

