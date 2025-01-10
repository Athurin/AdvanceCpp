#include "QhttpDemo.h"


QhttpDemo::QhttpDemo(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

}

//mainwindow
//QhttpDemo::QhttpDemo(QWidget *parent, QVector<Comment>& vCom)
//    : QMainWindow(parent)
//{
//    ui.setupUi(this);
//
//    tableView = new QTableView;
//    centralWidget = new QWidget;
//    centralLayout = new QVBoxLayout;
//    this->setCentralWidget(centralWidget);
//
//    initDataSource(vCom);
//
//    initTableView();
//
//    centralLayout->addWidget(tableView);
//    centralWidget->setLayout(centralLayout);
//
//}

void QhttpDemo::initwindow(QVector<Comment>& vCom)
{
    tableView = new QTableView;
    centralWidget = new QWidget;
    centralLayout = new QVBoxLayout;
    this->setCentralWidget(centralWidget);

    initDataSource(vCom);

    initTableView();

    centralLayout->addWidget(tableView);
    centralWidget->setLayout(centralLayout);
}



void QhttpDemo::initTableView()
{
    //创建自定义表格实例
    m_pModel = new TableModel(this);
    m_pModel->setContent(m_vector);  //设置模型内容
    

    tableView->setModel(m_pModel);  //设置模型到视图

    //水平表头的调整模式为拉伸（Stretch），垂直表头的调整模式为根据内容调整大小
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->verticalHeader()->hide();

    // 设置单击选中行
    //ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //关闭滚动条
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
}

QhttpDemo::~QhttpDemo()
{
    
}


void QhttpDemo::initDataSource(QVector<Comment>& vCom)  //想办法转换json数据
{
    int len = vCom.size();
    for (int i = 0; i < len; i++)
    {
        m_vector.append(vCom[i]);
    }

    qDebug() << "4. m_vector size:" << m_vector.size();
}