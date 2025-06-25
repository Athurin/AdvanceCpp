#ifndef __SETTINGS_H__
#define __SETTINGS_H__
//配置文件类 以后会封装为DLL，便于不同的项目之间的统一管理

#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <cstring>

//描述宏 表示传入参数和传出参数
#ifndef IN
#define IN
#endif

//描述宏 表示传入参数和传出参数
#ifndef OUT
#define OUT
#endif

//本地使用gbk编码，但是QString使用UTF16编码
inline QString fromChString(const char* str)
{
	return QString::fromLocal8Bit(str);
}

inline std::string toStdString(const QString& qstr)
{
	return qstr.toStdString();
}

//定义模板，转换泛型Variant
template<typename T>
/*inline*/ QVariant _toVariant(const T & data = T()) //可以接受形参为空的情况，提供默认值 T()
{
	return QVariant::fromValue(data);
}


class Settings
{
public:
	Settings(){}
	Settings(const QString& filepath);
	~Settings() {}

	/*
	配置文件是否存在
	*/
	bool contain() const;

	/*
	初始化，创建默认配置文件
	*/
	bool settingsInit();

	/*
	fun : 向配置文件写数据
	param domin : 域
	param key : 键
	param value : 值
	return : 写入成功后返回 true
	*/
	void write(const QString& domin, const QString& key, const QVariant& value);

	/*
	fun : 从配置文件读取参数
	param domin : 域
	param key : 键
	param value : 读取的参数 输出型
	return : 读取成功返回true
	*/
	void read(const QString& domin, const QString& key, OUT QVariant& value);
	


	/*----------------------------基本参数API------------------------------*/

	QString db_ServerIP  ();  //获取数据库服务的域名
	QString db_source    ();  //获取数据库的名称
	QString db_userName  ();  //获取数据库登录用户名
	QString db_password  ();  //获取数据库密码

	void set_db_ServerIP (const QString& value);  //修改数据库服务的域名
	void set_db_source   (const QString& value);  //修改数据库的名称
	void set_db_userName (const QString& value);  //修改数据库登录用户名
	void set_db_password (const QString& value);  //修改数据库密码

	
protected:
	QString m_filepath;

private:
	//一般说加载配置文件，指的是把磁盘文件的数据放到内存中
	//最简单的方法就是定义 成员变量 （数据加载到内存）
	//使用成员变量记录的时候要注意，内存和磁盘数据要时刻同步

	//这里不使用加载的方式，而是直接在文件中修改和读取，无需考虑同步的问题

};

#endif