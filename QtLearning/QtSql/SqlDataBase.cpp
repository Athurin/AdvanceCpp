#include "SqlDataBase.h"

//数据库连接的初始化
bool 
connectCreate
(const QString& conName, const QString& hostName, const QString& DBName, const QString& User, const QString& Password )
{
	QSqlDatabase db = QSqlDatabase::addDatabase(conName); //static  "连接名
	db.setHostName(hostName);
	db.setDatabaseName(DBName);
	db.setUserName(User);
	db.setPassword(Password);

	if (!db.open())
	{
		QString dbg = db.lastError().text();
		qDebug() << dbg;
		return false;
	}

	return true;
}