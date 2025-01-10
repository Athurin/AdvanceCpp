#pragma once

//mainwindow 具有tabelview视图
#include <QtWidgets/QMainWindow>
#include "ui_QhttpDemo.h"
#include <QScopedPointer>  // Qt 框架中的一个智能指针类模板
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableView>
#include <QHeaderView>
#include <QDebug>
#include "TableModel.h"
#include "Comment.h"



QT_BEGIN_NAMESPACE
namespace Ui { class QhttpDemo; }
QT_END_NAMESPACE

//前向声明
//减少依赖、避免循环依赖、减少编译时间
class Comment;
class QTableWidgetItem;


class QhttpDemo : public QMainWindow
{
    Q_OBJECT

public:
    QhttpDemo(QWidget* parent = nullptr);
    //QhttpDemo(QWidget *parent, QVector<Comment>& vCom);
    ~QhttpDemo();

    void initTableView();
    void initDataSource(QVector<Comment>& vCom);

    void initwindow(QVector<Comment>& vCom);

private:

    QVector<Comment> m_vector;   //装数据
    TableModel* m_pModel = nullptr;  //数据模型
    //BoxDelegate* m_pComboxDelegate = nullptr;

    QTableView* tableView = nullptr;  //表格视图组件，用于显示和编辑数据
    QWidget* centralWidget = nullptr;
    QVBoxLayout* centralLayout = nullptr;

    Ui::QhttpDemoClass ui;
};
