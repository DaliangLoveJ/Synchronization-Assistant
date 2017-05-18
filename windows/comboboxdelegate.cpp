#include "windows/comboboxdelegate.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QDebug>


ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem  viewOption(option);
    if (viewOption.state & QStyle::State_HasFocus)
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);

    int height = (viewOption.rect.height() - 14) / 2;
    QPixmap pixmap = QPixmap(":/title/delete");
    QRect decorationRect = QRect(viewOption.rect.left() + viewOption.rect.width() -30 , viewOption.rect.top() + height, 20, 15);
    painter->drawPixmap(decorationRect, pixmap);
}

bool ComboBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    int height = (option.rect.height() - 9) / 2;
    QRect decorationRect = QRect(option.rect.left() + option.rect.width() - 18, option.rect.top() + height, 9, 9);
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()) )
    {
        emit deleteItem(index);
    }

    if (event->type() == QEvent::MouseMove && decorationRect.contains(mouseEvent->pos()))
    {
        QCursor cursor(Qt::PointingHandCursor);
        QApplication::setOverrideCursor(cursor);
    }
    else
    {
        QCursor cursor(Qt::ArrowCursor);
        QApplication::setOverrideCursor(cursor);
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

