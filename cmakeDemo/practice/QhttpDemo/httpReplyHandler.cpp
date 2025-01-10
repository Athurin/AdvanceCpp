#include "httpReplyHandler.h"

httpReplyHandler::httpReplyHandler(QObject* parent)
{

}

httpReplyHandler::~httpReplyHandler()
{

}

//将响应的的结果转换成json数据
//reply指向网络应答的结果
void httpReplyHandler::onHandleHttpReply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();// 读取应答数据

        m_jsonDoc = QJsonDocument::fromJson(responseData);

        // 转换成json 字符串进行打印 测试连接
        QString jsonStr = m_jsonDoc.toJson(QJsonDocument::Indented);
        qDebug().noquote() << "Response data : " << jsonStr;

        if (!m_jsonDoc.isArray()) //顶层类不是数组
        {
            qWarning() << "Invalid Comment Data!";
            return;
        }

        PaserComment();

    }
}


//将m_jsonDoc转换为类Comment的实例 的方法
void httpReplyHandler::PaserComment()
{
    QJsonArray jsonArray = m_jsonDoc.array();
    for (const QJsonValue& jsonValue : jsonArray)
    {
        if (!jsonValue.isObject())  //数组元素是类
        {
            qWarning() << "Invalid user data";
            continue;
        }

        QJsonObject jsonObject = jsonValue.toObject(); 
        Comment comment;

        comment.setPostId(jsonObject["postId"].toInt());
        comment.setId(jsonObject["id"].toInt());
        comment.setName(jsonObject["name"].toString());
        comment.setEmail(jsonObject["email"].toString());
        comment.setBody(jsonObject["body"].toString());

        //qDebug() << "***********" << jsonObject["name"].toString();

        this->m_comments.push_back(comment);
    }
    qDebug() << "1. m_comments size:" << m_comments.size();

    emit commentsReady(m_comments);  //发出准备信号

}

//让外界复制一个m_comment的副本
QVector<Comment> httpReplyHandler::getComments() const
{
    qDebug() << "2. m_comments size:" << m_comments.size();
    return m_comments;
}