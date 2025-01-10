#include "TableModel.h"
#include "TableModel.h"
#include "Comment.h"
#include <QDebug>

TableModel::TableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

TableModel::~TableModel()
{
   
}

void TableModel::setContent(QVector<Comment> comment)
{
    m_comment = comment;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) //如果请求的是水平表头（列标题）,根据给定的section值返回列标题
    {
        switch (section)
        {
        case 0:
            return QString("PostID");
        case 1:
            return QString("ID");
        case 2:
            return QString("Name");
        case 3:
            return QString("Email");
        case 4:
            return QString("Body");

        default:
            break;

        }
    }
    return QVariant();
}

int TableModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_comment.size();
}

int TableModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : 5;
}

//QModelIndex类是用来唯一标识模型中的一个项（item） 可以分别获得行索引和列索引
QVariant TableModel::data(const QModelIndex& index, int role) const
{
    //检查index是否有效
    if (!index.isValid())
        return QVariant();

    //检查行索引是否有效
    if (index.row() >= m_comment.size() || index.row() < 0)
        return QVariant();

    // 返回显示数据或者编辑数据
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        const auto& comment = m_comment.at(index.row());  //index的行号就对应的是 m_comment中的对象的下标  
        //取出数组中的某一个元素，根据列的索引分别返回值

        switch (index.column())
        {
        case 0:
            return comment.getPostId();
            
        case 1:
            return comment.getId();

        case 2:
            return comment.getName();

        case 3:
            return comment.getEmail();

        case 4:
            return comment.getBody();

        default:
            break;
        }

    }
    return QVariant();
}

//只是显示请求结果而不修改数据的话，可以不用这个模块
//bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
//{
//    if (!index.isValid())
//        return false;
//
//    if (index.row() >= m_comment.size() || index.row() < 0)
//        return false;
//
//    if (role == Qt::EditRole) // 设置编辑时的数据
//    {
//        const auto& rq = m_comment.at(index.row());
//        switch (index.column())
//        {
//        case 0:
//            
//            break;
//        case 1:
//            
//            break;
//
//        default:
//            break;
//        }
//    }
//
//    return true;
//}

//设置索引为1的单元格的内容为可编辑的
//Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
//{
//    if (!index.isValid())
//        return Qt::NoItemFlags;
//    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
//    if (index.column() == 1)   //可以编辑索引为1的单元格的内容
//        return defaultFlags | Qt::ItemIsEditable; // 对于第一列增加编辑标志
//    else
//        return defaultFlags;
//
//}