#include "windows/userdatamodel.h"
#include "functions/common.h"
#include <QCheckBox>
#include <QApplication>
#include <QMouseEvent>






TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

TableModel::~TableModel()
{
}

// 更新表格数据
void TableModel::updateData(QList<FileRecord> recordList)
{
    m_recordList = recordList;
    beginResetModel();
    endResetModel();
}

int TableModel::rowNumber() const
{
    return m_recordList.count();
}

int TableModel::rowCount(const QModelIndex &index) const
{

    return m_recordList.count();
}

int TableModel::columnCount(const QModelIndex &index) const
{
    return 6;
}

// 设置表格项数据
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int nColumn = index.column();
    FileRecord record = m_recordList.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    {
        if (nColumn == FILE_NAME_COLUMN)
        {
            record.strFileName = value.toString();
        }
        else if (nColumn == DATE_TIME_COLUMN)
        {
            record.dateTime = value.toDateTime();
        }
        else if (nColumn == DATE_TIME_COLUMN)
        {
            record.dateTime = value.toDateTime();
        }
        else if (nColumn == FILE_PATH_COLUMN)
        {
            record.strFilePath= value.toString();
        }
        else if (nColumn == FILE_COPY_COLUMN)
        {
            record.copyStart= value.toString();
        }
        m_recordList.replace(index.row(), record);
        emit dataChanged(index, index);
        return true;
    }
    case Qt::CheckStateRole:
    case Qt::UserRole:
    {
        if (nColumn == CHECK_BOX_COLUMN)
        {
            record.bChecked = value.toBool();

            m_recordList.replace(index.row(), record);
            emit dataChanged(index, index);
            return true;
        }
    }
    default:
        return false;
    }
    return false;
}

// 表格项数据
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    FileRecord record = m_recordList.at(nRow);

    switch (role)
    {
    case Qt::TextColorRole:
        return QColor(Qt::white);
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (nColumn == FILE_NAME_COLUMN)
        {
            return record.strFileName;
        }
        else if (nColumn == DATE_TIME_COLUMN)
        {
            return record.dateTime;
        }
        else if (nColumn == FILE_SIZE_COLUMN)
        {
            return record.nSize;
        }
        else if (nColumn == FILE_PATH_COLUMN)
        {
            return record.strFilePath;
        }
        else if (nColumn == FILE_COPY_COLUMN)
        {
            return record.copyStart;
        }
        return "";
    }
    case Qt::UserRole:
    {
        if (nColumn == CHECK_BOX_COLUMN)
            return record.bChecked;
    }

    default:
        return QVariant();
    }

    return QVariant();
}

// 表头数据
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == CHECK_BOX_COLUMN)
                return QStringLiteral("状态");

            if (section == FILE_NAME_COLUMN)
                return QStringLiteral("文件名");

            if (section == DATE_TIME_COLUMN)
                return QStringLiteral("修改日期");
            if (section == FILE_SIZE_COLUMN)
                return QStringLiteral("大小(KB)");
            if (section == FILE_PATH_COLUMN)
                return QStringLiteral("存储路径");
            if (section == FILE_COPY_COLUMN)
                return QStringLiteral("当前状态");
        }
    }
    default:
        return QVariant();
    }

    return QVariant();
}

/*设置表格可以选中*/
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return flags;
}



CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

CheckBoxDelegate::~CheckBoxDelegate()
{

}



void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    if (index.column() == CHECK_BOX_COLUMN)
    {
        bool data = index.model()->data(index, Qt::UserRole).toBool();
        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.iconSize = QSize(20, 20);
        checkBoxStyle.rect = option.rect;
        QCheckBox checkBox;
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
    }
}


bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect decorationRect = option.rect;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
    {
        if (index.column() == CHECK_BOX_COLUMN)
        {
            bool data = model->data(index, Qt::UserRole).toBool();
            model->setData(index, !data, Qt::UserRole);
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}


/*---------------------------------------------------------*/
//TableHeaderView::TableHeaderView(Qt::Orientation orientation, QWidget *parent)
//    : QHeaderView(orientation, parent),
//      m_bPressed(false),
//      m_bChecked(false),
//      m_bTristate(false),
//      m_bNoChange(false),
//      m_bMoving(false)
//{
//    // setStretchLastSection(true);
//    setHighlightSections(false);
//    setMouseTracking(true);

//    // 响应鼠标
//    setSectionsClickable(true);
//}

//// 槽函数，用于更新复选框状态
//void TableHeaderView::onStateChanged(int state)
//{
//    if (state == Qt::PartiallyChecked) {
//        m_bTristate = true;
//        m_bNoChange = true;
//    } else
//    {
//        m_bNoChange = false;
//    }

//    m_bChecked = (state != Qt::Unchecked);
//    update();
//}

//// 绘制复选框
//void TableHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
//{
//    painter->save();
//    QHeaderView::paintSection(painter, rect, logicalIndex);
//    painter->restore();

//    if (logicalIndex == CHECK_BOX_COLUMN)
//    {
//        QStyleOptionButton option;
//        option.initFrom(this);

//        if (m_bChecked)
//            option.state |= QStyle::State_Sunken;

//        if (m_bTristate && m_bNoChange)
//            option.state |= QStyle::State_NoChange;
//        else
//            option.state |= m_bChecked ? QStyle::State_On : QStyle::State_Off;
//        if (testAttribute(Qt::WA_Hover) && underMouse()) {
//            if (m_bMoving)
//                option.state |= QStyle::State_MouseOver;
//            else
//                option.state &= ~QStyle::State_MouseOver;
//        }

//        QCheckBox checkBox;
//        option.iconSize = QSize(20, 20);
//        option.rect = rect;
//        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, &checkBox);
//        //style()->drawItemPixmap(painter, rect, Qt::AlignCenter, QPixmap(":/images/checkBoxChecked"));
//        //style()->drawControl(QStyle::CE_CheckBox, &option, painter, this);
//    }
//}

//// 鼠标按下表头
//void TableHeaderView::mousePressEvent(QMouseEvent *event)
//{
//    int nColumn = logicalIndexAt(event->pos());
//    if ((event->buttons() & Qt::LeftButton) && (nColumn == CHECK_BOX_COLUMN))
//    {
//        m_bPressed = true;
//    }
//    else
//    {
//        QHeaderView::mousePressEvent(event);
//    }
//}

//// 鼠标从表头释放，发送信号，更新model数据
//void TableHeaderView::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (m_bPressed)
//    {
//        if (m_bTristate && m_bNoChange)
//        {
//            m_bChecked = true;
//            m_bNoChange = false;
//        }
//        else
//        {
//            m_bChecked = !m_bChecked;
//        }

//        update();

//        Qt::CheckState state = m_bChecked ? Qt::Checked : Qt::Unchecked;

//        emit stateChanged(state);
//    }
//    else
//    {
//        QHeaderView::mouseReleaseEvent(event);
//    }

//    m_bPressed = false;
//}

//// 鼠标滑过、离开，更新复选框状态
//bool TableHeaderView::event(QEvent *event)
//{
//    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave)
//    {
//        QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
//        int nColumn = logicalIndexAt(pEvent->x());
//        if (nColumn == CHECK_BOX_COLUMN)
//        {
//            m_bMoving = (event->type() == QEvent::Enter);

//            update();
//            return true;
//        }
//    }
//    return QHeaderView::event(event);
//}
