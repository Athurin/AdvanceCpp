#include <iostream>
#include "Settings.h"
#include "SqlDataBase.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv); // 创建 QCoreApplication 实例

	// 检查可用驱动
	qDebug() << "Available drivers:" << QSqlDatabase::drivers();

	// 	QString pluginPath = "D:/Qt/5.14.2/msvc2017_64/plugins/sqldrivers";
	// 	app.addLibraryPath(pluginPath);
	// QCoreApplication::setLibraryPaths(QStringList() << "D:/Qt/qt_app/5.14.2/msvc2017_64/plugins/sqldrivers");

	// 检查库路径
	//qDebug() << "Library paths:" << app.libraryPaths();

	Settings settings(fromChString("settings.ini")); //生成的时候默认在.sln文件同级目录下
	settings.settingsInit();

	bool hResult = connectCreate(fromChString("QMYSQL"), fromChString("default1"), \
		settings.db_ServerIP(), settings.db_source(), settings.db_userName(), settings.db_password());
	if (hResult) std::cout << "连接成功";
	else std::cout << "连接失败";

	app.exec();

	return 0;
}
