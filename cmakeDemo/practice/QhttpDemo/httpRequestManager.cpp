#include "httpRequestManager.h"

httpRequestManager::httpRequestManager(QObject* parent)
{
	//信号链接信号
	(void)QObject::connect(&m_httpManager, &QNetworkAccessManager::finished, this, &httpRequestManager::requestFinished);
}

httpRequestManager::~httpRequestManager()
{

}

//向指定的url发送http get请求
void httpRequestManager::sendGetRequest(const QUrl& url)
{
	QNetworkRequest request(url);
	m_httpManager.get(request);//m_httpManager是一个QNetworkAccessManager对象，负责管理网络请求和响应
	//get方法会发起一个HTTP GET请求，对应的QNetworkRequest对象包含了请求的详细信息
}