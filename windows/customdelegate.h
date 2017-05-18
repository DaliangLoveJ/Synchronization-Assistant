#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QStyledItemDelegate>
class QPushButton;


class CustomDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomDelegate(QWidget *parent = 0);
    ~CustomDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

signals:
    void open(const QModelIndex &index);

private:
    QPoint m_mousePoint;  // 鼠标位置
    QScopedPointer<QPushButton> outlookButton;
    QScopedPointer<QPushButton> foxmailButton;
    int m_nWidth;   // 按钮宽度
    int m_nHeight;  // 按钮高度
    int m_nType;    // 按钮状态-1：划过 2：按下
};
#endif // SRCBOXDELEGATE_H


