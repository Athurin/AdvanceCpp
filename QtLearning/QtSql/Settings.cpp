#include "Settings.h"


Settings::Settings(const QString& filepath)
	:m_filepath(filepath)
{
	
}

/*
	配置文件是否存在
*/
bool Settings::contain() const
{
	return QFile::exists(m_filepath);
}


//配置文件初始化  [database]
bool Settings::settingsInit()
{
	bool res = contain();
	if (!res) //如果配置文件不存在，就创建一个QSettings对象，帮助生成和初始化配置文件
	{
		QSettings settings(m_filepath, QSettings::IniFormat); //没有文件就创建一个
		settings.beginGroup(fromChString("dataBase"));
		//settings.setValue(fromChString("ServerIP"), _toVariant<int>()); 实例，参数可以为空值，但是要指定模板类型
		//const char* 直接转为QVariant会出现错误，需要先注册； 也可以直接转为QString，再转QVariant
		settings.setValue(fromChString("ServerIP"), _toVariant(fromChString("127.0.0.1")));
		settings.setValue(fromChString("source"), _toVariant(fromChString("huangpishixun")));
		settings.setValue(fromChString("userName"), _toVariant(fromChString("root")));
		settings.setValue(fromChString("password"), _toVariant(fromChString("root123456")));

		settings.endGroup();
	}
	return res;
}

void Settings::write(const QString& domin, const QString& key, const QVariant& value)
{
	QSettings settings(m_filepath, QSettings::IniFormat); //配置文件管理器可以在需要的时候创建并初始化
	//坏处是每次读取都要初始化，好处是不用检查文件路径是否正确
	settings.setValue(domin + '/' + key, value);
}

void Settings::read(const QString& domin, const QString& key, OUT QVariant& value)
{
	QSettings settings(m_filepath, QSettings::IniFormat);
	value = settings.value(domin + '/' + key);
}

QString Settings::db_ServerIP()  //获取数据库服务的域名
{
	QVariant v;
	read(fromChString("dataBase"), fromChString("ServerIP"), v);
	return v.toString();
}

QString Settings::db_source()  //获取数据库的名称
{
	QVariant v;
	read(fromChString("dataBase"), fromChString("source"), v);
	return v.toString();
}

QString Settings::db_userName()  //获取数据库登录用户名
{
	QVariant v;
	read(fromChString("dataBase"), fromChString("userName"), v);
	return v.toString();
}

QString Settings::db_password()  //获取数据库密码
{
	QVariant v;
	read(fromChString("dataBase"), fromChString("password"), v);
	return v.toString();
}

void Settings::set_db_ServerIP(const QString& value)  //修改数据库服务的域名
{
	write(fromChString("dataBase"), fromChString("ServerIP"), value);
}

void Settings::set_db_source(const QString& value)  //修改数据库的名称
{
	write(fromChString("dataBase"), fromChString("source"), value);
}

void Settings::set_db_userName(const QString& value)  //修改数据库登录用户名
{
	write(fromChString("dataBase"), fromChString("userName"), value);
}

void Settings::set_db_password(const QString& value)  //修改数据库密码
{
	write(fromChString("dataBase"), fromChString("password"), value);
}