#pragma once
#ifndef __TABLEMODEL_H_
#define __TABLEMODEL_H_
#include <QAbstractTableModel>
#include <QVector>

class Comment;

class TableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	TableModel(QObject* parent);
	~TableModel();

	void setContent(QVector<Comment> comment);

	//返回表头标题内容
	//Qt::Orientation是一个枚举类型，可以是Qt::Horizontal（水平）或Qt::Vertical（垂直）  指定了表头的朝向
	//section:对于水平表头（通常是列标题），section指的是列的索引；对于垂直表头（通常是行标题），section指的是行的索引
	//这个参数指定了请求的数据的角色 DisplayRole表示请求用于展示的数据
	virtual QVariant
		headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;

	//返回模型的行数
	//QModelIndex是一个索引类，用于唯一标识模型中的一个项（item）
	virtual int
		rowCount(const QModelIndex& parent = QModelIndex()) const override;

	//返回模型的列数
	virtual int
		columnCount(const QModelIndex& parent = QModelIndex()) const override;

	//根据模型搜索与数据角色，返回模型数据
	virtual QVariant
		data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	// 设置模型数据，一般由代理来负责调用
	//virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	// 指定单元格设置一些标志
	//virtual  Qt::ItemFlags flags(const QModelIndex& index) const;


private:
	QVector<Comment> m_comment;



};

#endif 