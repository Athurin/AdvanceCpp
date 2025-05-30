#ifndef __QTBYTEARRAY_H__
#define __QTBYTEARRAY_H__
#include <iostream>
//#include <QtCore/QTextStream>
#include <QtCore/QByteArray>
#include <QtCore/QChar>
#include <QtCore/QString>

//测验Qt写时复制
void testFromRawData()
{
	const char* cstr = "QtByteArray-readOnly"; //原始数据 raw data
	int size = 10;

	QByteArray qBa = QByteArray::fromRawData(cstr, size); //只会使用前size个字符串
	QByteArray qBb = QByteArray::fromRawData(cstr, strlen(cstr));
	//fromRawData()船舰的实例不是默认以 '\0' 结尾的， 除非 raw data 单重含有'\0'

	void* constPtrA = (void*)qBa.constData(); //获取首地址
	void* constPtrB = (void*)qBb.constData(); //获取首地址

	std::cout << "qBa data: " << qBa.constData() << std::endl;
	std::cout << "qBa Ptr : " << constPtrA << std::endl;
	std::cout << "qBa size: " << qBa.size() << std::endl;

	std::cout << "qBb data: " << qBb.constData() << std::endl;
	std::cout << "qBa Ptr : " << constPtrB << std::endl;
	std::cout << "qBb size: " << qBb.size() << std::endl;

	qBa[1] = 'T'; //强行修改，写时复制

	//再次获取地址
	constPtrA = (void*)qBa.constData(); //获取首地址
	constPtrB = (void*)qBb.constData(); //获取首地址

	std::cout << "After- qBa data: " << qBa.constData() << std::endl;
	std::cout << "After- qBa Ptr : " << constPtrA << std::endl;
	std::cout << "After- qBa size: " << qBa.size() << std::endl;

	std::cout << "After- qBb data: " << qBb.constData() << std::endl;
	std::cout << "After- qBa Ptr : " << constPtrB << std::endl;
	std::cout << "After- qBb size: " << qBb.size() << std::endl;
}

//测验Qt的IndexOf 和 lastINdexOf的区别
void findIndex()
{
	QByteArray ba("We must be <b>bold</b>, very <b>bold</b>");
	int j = 0;
	while ((j = ba.indexOf("<b>", j)) != -1)   //从 j 开始查找第一个出现的下标， 并赋值给 j
	{
		//Qt::endl是为QDebug设计的，不是标准 C++ 的输出流。在标准流当中插入Qt::endl;会出现乱码
		//std::cout << "Found <b> tag at index position : " << j << Qt::endl << Qt::endl << Qt::endl << Qt::endl << Qt::endl;  //需要包含#include <QtCore/QTextStream>
		
		std::cout << "Found <b> tag at index position : " << j << std::endl << std::endl;
		
		++j;
	}



	//j = -1;
	j = ba.size() -1;  //两种初始化的方法是等价的
	while ((j = ba.lastIndexOf("<b>", j)) != -1)  //从末尾开始查找
	{
		std::cout << "Found <b> tag at index position : " << j << std::endl << std::endl;

		j--;
	}


	//顺序
	QByteArray x("crazy azimuths");
	QByteArray y("az");
	std::cout << x.indexOf(y) << std::endl;                           // returns 2      查找范围 crazy azimuths
	std::cout << x.indexOf(y, 6) << std::endl;                        // returns 6      查找范围 zimuths
	std::cout << x.indexOf(y, 7) << std::endl << std::endl;           // returns -1      查找范围 azimuths



	//倒序
	//需要注意第二种情况
	std::cout << x.lastIndexOf(y) << std::endl;           // returns 6      查找范围 crazy azimuths
	std::cout << x.lastIndexOf(y, 6) << std::endl;        // returns 6      查找范围 crazy a 
	std::cout << x.lastIndexOf(y, 5) << std::endl;        // returns 2      查找范围 crazy 
	std::cout << x.lastIndexOf(y, 1) << std::endl;        // returns -1     查找范围 cr

}


//QbyteArray和QString的比较有什么不同
void QByteVSQString () 
{
	// Latin-1 字符
	QByteArray latinArray = "Straße"; // 德语中的 "street"

	//Unicode's
	QString unicodeString = QString::fromLatin1("Straße");

	// 转换为小写
	QByteArray lowerLatin = latinArray.toLower(); // 结果可能不正确，因为 'ß' 在 Latin-1 中没有小写形式
	QString lowerUnicode = unicodeString.toLower(); // 正确处理 'ß'，结果为 "straße"

	std::cout << "QByteArray toLower(): " << lowerLatin.constData() << std::endl;
	std::cout << "QString toLower(): " << lowerUnicode.toStdString() << std::endl;

	// 比较字符
	char c1 = 'A';
	char c2 = 'a';
	std::cout << "QByteArray comparison (A vs a): " << (c1 < c2 ? "A < a" : "A >= a") << std::endl; // Latin-1 中 'A'（65）小于 'a'（97）

	QChar qc1 = 'A';
	QChar qc2 = 'a';
	std::cout << "QString comparison (A vs a): " << (qc1 < qc2 ? "A < a" : "A >= a") << std::endl; // Unicode 中 'A'（U+0041）小于 'a'（U+0061）

}

//Base64 Coding and EnCoding
void Base64() 
{
	QByteArray data = "https://www.qt.io?/=%/Hello+?/World!?/";

	// 标准 Base64 编码
	QByteArray standardBase64 = data.toBase64();
	std::cout << "Standard Base64: " << standardBase64.constData() << std::endl; //编码
	QByteArray test1 = QByteArray::fromBase64(standardBase64); //解码
	std::cout << "Standard Base64 Encoding :" << test1.constData() << std::endl;
	std::cout << std::endl;

	// URL 安全的 Base64 编码
	QByteArray urlBase64 = data.toBase64(QByteArray::Base64UrlEncoding);
	std::cout << "URL Base64: " << urlBase64.constData() << std::endl;
	QByteArray test2 = QByteArray::fromBase64(urlBase64, QByteArray::Base64UrlEncoding); //解码
	std::cout << "urlBase64 Encoding :" << test2.constData() << std::endl;
	std::cout << std::endl;

	// 带等号的 URL 安全 Base64 编码
	QByteArray urlBase64Padding = data.toBase64(QByteArray::Base64UrlEncoding | QByteArray::KeepTrailingEquals);
	std::cout << "URL Base64 (No Padding): " << urlBase64Padding.constData() << std::endl;
	QByteArray test3 = QByteArray::fromBase64(urlBase64Padding, QByteArray::Base64UrlEncoding);
	std::cout << "Decoded URL Base64 (With Padding): " << test3.constData() << std::endl;
	std::cout << std::endl;

	// 省略尾部等号的 URL 安全 Base64 编码
	QByteArray urlBase64NoPadding = data.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
	std::cout << "URL Base64 (No Padding): " << urlBase64NoPadding.constData() << std::endl;
	QByteArray test4 = QByteArray::fromBase64(urlBase64NoPadding, QByteArray::Base64UrlEncoding);
	std::cout << "Decoded URL Base64 (With Padding): " << test4.constData() << std::endl;
	std::cout << std::endl;

}


#endif