#include <iostream>
#include "Settings.h"
#include "SqlDataBase.h"

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv); // 创建 QCoreApplication 实例
	

	Settings settings(fromChString("settings.ini")); //生成的时候默认在.sln文件同级目录下
	settings.settingsInit();

	bool hResult = connectCreate(  fromChString("con0625"), \
		settings.db_ServerIP(), settings.db_source(), settings.db_userName(), settings.db_password());
	if (hResult) std::cout << "连接成功";
	else std::cout << "连接失败";

	app.exec();

	return 0;
}