#include <QApplication>
#include <QCheckBox>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QToolTip>
#include <QEvent>
#include <QMouseEvent>
#include "windows/customdelegate.h"

CustomDelegate::CustomDelegate(QWidget *parent)
    : QStyledItemDelegate(parent),
      outlookButton(new QPushButton()),
      foxmailButton(new QPushButton()),
      m_nWidth(60),
      m_nHeight(22)
{
    // 设置按钮正常、划过、按下样式
    outlookButton->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/appwin/outlookadd);} \
                                  QPushButton:hover {image:url(:/appwin/doutlookadd);} \
                                  QPushButton:pressed {image:url(:/appwin/doutlookadd);}");
    foxmailButton->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/appwin/foxmailstart);} \
                                  QPushButton:hover {image:url(:/appwin/dfoxmailstart);} \
                                  QPushButton:pressed {image:url(:/appwin/dfoxmailstart);}");

}

CustomDelegate::~CustomDelegate()
{
}

// 绘制按钮
void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    if(index.data().toString() == "Microsoft Outlook")
    {
        int nHalf = (option.rect.width() - m_nWidth  ) / 2;
        int nTop = (option.rect.height() - m_nHeight) / 2;
        // 绘制按钮
        QStyleOptionButton button;
        button.rect = QRect(option.rect.left() + nHalf + m_nWidth*5 , option.rect.top() + nTop,  m_nWidth, m_nHeight);
        button.state |= QStyle::State_Enabled;

        if (button.rect.contains(m_mousePoint))
        {
            if (m_nType == 0)
            {
                button.state |= QStyle::State_MouseOver;
            }
            else if (m_nType == 1)
            {
                button.state |= QStyle::State_Sunken;
            }
        }
        QWidget *outlookWidget =  outlookButton.data() ;
        outlookWidget->style()->drawControl(QStyle::CE_PushButton, &button, painter, outlookWidget);
    }

    QStyleOptionViewItem viewFOption(option);
    initStyleOption(&viewFOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewFOption.state = viewFOption.state ^ QStyle::State_HasFocus;
    QStyledItemDelegate::paint(painter, viewFOption, index);
    if(index.data().toString() == "Foxmail 7")
    {
        int nFHalf = (option.rect.width() - m_nWidth  ) / 2;
        int nFTop = (option.rect.height() - m_nHeight) / 2;
        // 绘制按钮
        QStyleOptionButton Fbutton;
        Fbutton.rect = QRect(option.rect.left() + nFHalf + m_nWidth * 5 , option.rect.top() + nFTop,  m_nWidth, m_nHeight);
        Fbutton.state |= QStyle::State_Enabled;

        if (Fbutton.rect.contains(m_mousePoint))
        {
            if (m_nType == 0)
            {
                Fbutton.state |= QStyle::State_MouseOver;
            }
            else if (m_nType == 1)
            {
                Fbutton.state |= QStyle::State_Sunken;
            }
        }
        QWidget *foxmailWidget =  foxmailButton.data() ;
        foxmailWidget->style()->drawControl(QStyle::CE_PushButton, &Fbutton, painter, foxmailWidget);
    }

}

/* 响应按钮事件 - 划过、按下 */
bool CustomDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    m_nType = -1;
    bool bRepaint = false;
    QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
    m_mousePoint = pEvent->pos();
    QStyledItemDelegate::editorEvent( event,  model, option, index);

    int nHalf = (option.rect.width() - m_nWidth  ) / 2;
    int nTop =  (option.rect.height() - m_nHeight) / 2;
    QStyleOptionButton button;
    button.rect = QRect(option.rect.left()+nHalf+m_nWidth*5 , option.rect.top()+nTop,  m_nWidth, m_nHeight);

    /*鼠标位于按钮之上*/
    if (!button.rect.contains(m_mousePoint))
        return false;

    bRepaint = true;
    switch (event->type())
    {
        // 鼠标滑过
        case QEvent::MouseMove:
        {
            m_nType = 0;
            break;
        }
        // 鼠标按下
        case QEvent::MouseButtonPress:
        {
            m_nType = 1;
            break;
        }
        // 鼠标释放
        case QEvent::MouseButtonRelease:
        {
            emit open(index);
            break;
        }
        default:
            break;
    }
    return bRepaint;
}
