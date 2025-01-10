#pragma once
#ifndef __HTTP_REQUESTMANAGER_H__
#define __HTTP_REQUESTMANAGER_H__

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class httpRequestManager : public QObject
{
	Q_OBJECT
public:
	httpRequestManager(QObject* parent = nullptr);
	~httpRequestManager();

	void sendGetRequest(const QUrl& url);


signals:

	//finished() 信号有一个参数，即 QNetworkReply* 类型的指针，指向完成的网络请求的回复对象
	//所以在传递信号的时候一定要指示参数

	//需要包含QNetworkReply类的完整定义 使用#include<>，而不是class+类名
	void requestFinished(QNetworkReply* reply);

private:
	QNetworkAccessManager m_httpManager;

};

#endif // !__HTTP_REQUESTMANAGER_H__