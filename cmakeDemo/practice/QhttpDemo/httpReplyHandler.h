#pragma once
#ifndef __HTTP_REPLYHANDLER_H__
#define __HTTP_REPLYHANDLER_H__

#include <QObject>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>  
#include <QJsonArray>  
#include <QDebug>  
#include <QtGlobal>
#include "Comment.h"

class httpReplyHandler : public QObject
{
    Q_OBJECT

signals:
    void commentsReady(QVector<Comment>);

public:
    httpReplyHandler(QObject* parent = nullptr);
    ~httpReplyHandler();

    void onHandleHttpReply(QNetworkReply* reply);

    //让外界复制一个m_comment 副本
    QVector<Comment> getComments() const;

private:
    QJsonDocument m_jsonDoc;
    QVector<Comment> m_comments;

    //将m_jsonDoc转换为类Comment的实例 的方法
    void PaserComment();



};

#endif