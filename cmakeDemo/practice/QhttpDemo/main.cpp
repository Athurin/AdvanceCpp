#include "QhttpDemo.h"
#include <QtWidgets/QApplication>
#include "httpRequestManager.h"
#include "httpReplyHandler.h"
#include "TableModel.h"
#include "Comment.h"
#include <QObject>
#include <QVector>
#include <QTimer>

//void Ready(QVector<Comment>& comReply)
//{
//
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if 0
    QhttpDemo w;
    w.show();
#endif

    httpRequestManager httpReqManager;  //管理http请求
    httpReplyHandler httpRlyHandler;  //管理http应答

    //QVector<Comment> comReply;  //评论数据
    QhttpDemo w;

    (void)QObject::connect(&httpReqManager, &httpRequestManager::requestFinished,
        &httpRlyHandler, &httpReplyHandler::onHandleHttpReply);

    QEventLoop loop;  // 创建事件循环

    httpReqManager.sendGetRequest(QUrl("https://jsonplaceholder.typicode.com/comments"));

    QObject::connect(&httpRlyHandler, &httpReplyHandler::commentsReady,
    [&loop, &w](QVector<Comment> http_comments) {
        w.initwindow(http_comments);
        loop.quit();  // 退出事件循环
    });


    w.show();
    
#if 0
    qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
#endif

    return a.exec();
}
