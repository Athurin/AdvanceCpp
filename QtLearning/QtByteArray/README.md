# 简单描述

## 可重入性

**`QByteArray`类中的所有方法都是可重入的，但不一定是线程安全的。**

> ## Reentrancy and Thread-Safety

Qt中标记了这两个特性的类都可以使用在多线程中。

区别是：

函数 `function`层面：
`Reentrancy` :  可重入性。表示多个线程都可以同时调用这个函数，但是每一个调用请求都必须使用自己的数据。
`Thread-Safety` : 线程安全性。表示多个线程都可以同时使用这个函数，并且每一个调用请求使用的是共享的数据，因为这些数据的引用是序列化的，也就是可串行的。

类 `class`层面：
`Reentrancy` :  可重入性。多个线程可以同时调用这个类的同一个成员函数，只要每个线程使用这个类的不同的实例。
`Thread-Safety` : 线程安全性。多个线程可以同时调用这个类的同一个成员函数，并且这些线程共享这个类的同一个实例。

==线程安全一定是可重入的，可重入的不总是线程安全的==

> [!注意]
> 而具有以下形态的类则是不可重入的：
>
> 1. 具有静态数据成员；
> 2. 某个或者某些成员函数的内部定义了静态局部变量；
> 3. 某个数据成员是指针，而该类多个对象的这个指针指向同一块内存。

Tips:

## **使用隐式共享（写时复制）**

**` QBtyeArray`总是确保以 `\0` 结尾，因此使用起来比 `const char*` 方便很多。**

使用隐式共享（`[implicit sharing](implicit-sharing.html) (copy-on-write)`）的方式去减少内存开销和赋值操作需要的开销。

> ### 隐式共享（Implicit Sharing）

* 隐式共享是一种资源共享机制，允许多个对象同时共享同一份数据，前提是这些对象都以只读方式访问数据，不会对数据进行修改。在这种情况下，多个对象可以共享同一份数据副本，从而减少内存占用和数据复制操作，提高程序的效率。
* 例如，在 Qt 的 QByteArray 或 QString 等类中，当一个对象被复制时，实际上并不会立即创建一份全新的数据副本。取而代之的是，多个对象会共享同一块内存区域中的数据。这种共享是隐式的，对用户来说通常是透明的，无需显式地进行共享操作。

> ### 写时复制（Copy-on-Write）

* 写时复制是隐式共享的一种实现方式。在这种机制下，当多个对象共享同一份数据时，数据的物理副本只在真正需要对数据进行写操作（即修改数据）时才会被创建。
* 具体来说，当一个对象尝试修改共享数据时，系统会自动检测到这一操作并触发写时复制过程。此时，会为该对象创建一份数据的副本，使其拥有自己独立的数据副本进行修改，而其他共享该数据的对象仍然继续使用原始数据副本。这样可以确保每个对象的数据独立性，同时避免了不必要的数据复制。

## 更适合QByteArray的场景

`QByteArray` 中的每个元素都是一个 8 位的字符码。而 `QString` 则存储16bit的Unicode编码，使之更适于存储非ASCII编码和非拉丁语系的字符。一般情况下使用 `QString` ，但是也有特殊情况：

**有两个重要场景更适合 `QByteArray` ：**

1. 需要存储二进制数据。
2. 对内存的要求非常严格，需要精细地管理。

## 如何初始化

传递一个 `const char*` 类型的数据就能做到初始化。

```cpp
  QByteArray ba("Hello");
```

初始化之后可以看到长度 `size() == 5` 但是末尾依然会隐式加上 `\0`。

`QByteArray`对一个 `const char*` 类型的数据总是发生**深拷贝**并且**修改时没有副作用。**

如果不想深拷贝，可以使用 `QByteArray::fromRawData` (这个方法创建的实例总是只读的，因此要保证原来的副本的生命周期大于 `QByteArray`实例，否则会导致未定义行为。如果想要修改数据，就会触发写时复制。)

**可以使用下标运算符 `[]` ，返回的是对元素的左值引用，可以读写对应的数据。**

**还有一个选择就是，使用 `at()` 运算符，但是返回的是只读的。**

`at()` 方法总是比 `[]` 快，是因为它从来不会导致深拷贝的发生。

`To extract many bytes at a time, use [left][right](), or [mid].`

**`QByteArray` 可以在字符串的中间嵌入 `\0。`**

**在调用 `resize()` 之后，会出现刚分配的控件里面的值是未初始化的，这是可以调用 `fill()` 函数实现所有元素的统一初始化。**

想要获取数据的头指针可以使用 `data()` 和 `constData()` 。这个头指针会一直有效，直到调用了非 `const` 方法。

> It is also guaranteed that the data ends with a '\0' byte unless the QByteArray was created from a [raw data](qbytearray.html#fromRawData).
>
> This '\0' byte is automatically provided by QByteArray and is not counted in [size](qbytearray.html#size)().

终止符 `\0` 是不会在 `size()`中计算的。

**当使用 `append` 给一个非空数据后面追加的时候，分为**

1. 原数组主动分配多余的空间
2. 将追加的数据**拷贝**到后面的空间。

可以使用 `capacity` 函数查看究竟分配了多少空间。

**在空的数据后面追加，就不是拷贝数据这么简单了。**

> Data appended to an empty array is not copied.

**移除所有空白字符 `('\n', '\t', ' ', etc.) `使用  `trimmed` **

**替换所有空白字符 `('\n', '\t', ' ', etc.) `为一个空格字符，使用 `simplified` **


## 查找

查找字串出现的下标。

```cpp
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
```

输出结果是：

```bash

Found <b> tag at index position : 11

Found <b> tag at index position : 29

Found <b> tag at index position : 29

Found <b> tag at index position : 11
```


**顺序**

```cpp
//顺序
QByteArray x("crazy azimuths");
QByteArray y("az");
std::cout << x.indexOf(y) << std::endl;                           // returns 2      查找范围 crazy azimuths
std::cout << x.indexOf(y, 6) << std::endl;                        // returns 6      查找范围 zimuths
std::cout << x.indexOf(y, 7) << std::endl << std::endl;           // returns -1      查找范围 azimuths
```

**倒序  和顺序查找略有不同**

只要查找后，匹配的结果的首字符的下标 `index`出现在查找范围内，无论是否所有字串都在查找范围内，都会返回这个下标 `index`

```cpp
//需要注意第二种情况
QByteArray x("crazy azimuths");
QByteArray y("az");
std::cout << x.lastIndexOf(y) << std::endl;           // returns 6      查找范围 crazy azimuths
std::cout << x.lastIndexOf(y, 6) << std::endl;        // returns 6      查找范围 crazy a 
std::cout << x.lastIndexOf(y, 5) << std::endl;        // returns 2      查找范围 crazy 
std::cout << x.lastIndexOf(y, 1) << std::endl;        // returns -1     查找范围 cr
```


## 比较

内部重载了方法 `operator<(), operator<=(), operator==(), operator>=()` 等，来完成字符串的比较。

字符的比较是基于字符编码的值来实现的。

* `QString::localeAwareCompare()` 是一个更好的选择，用于对用户界面中的字符串进行排序。
* 这个方法会考虑当前系统的语言和地区设置（locale），从而按照人类的预期进行比较和排序。例如：

  * 它会区分大小写（并且可以根据 locale 决定大小写的优先级）。
  * 它会正确处理带重音符号的字符和其他语言特定的字符。
  * 它会根据语言习惯调整排序规则（例如，德语中 "ß" 的处理方式）。

## Null 和 Empty

**Null Array :** 一个 `QbyteArray`实例是默认构造出来的或者传递 `(const char *)0` 给构造函数。

**Empty Array：** 一个长度为 `0` 的 `QByteArray` 。

> A null byte array is always empty, but an empty byte array isn't necessarily null.

```cpp

  QByteArray().isNull();          // returns true
  QByteArray().isEmpty();         // returns true

  QByteArray("").isNull();        // returns false
  QByteArray("").isEmpty();       // returns true

  QByteArray("abc").isNull();     // returns false
  QByteArray("abc").isEmpty();    // returns false
```

所有函数（除了 `isNull()`）都将（null byte array）和（empty byte array）视为相同的。例如，`data()` 函数会返回一个有效的指针（不是 `nullptr`），指向一个 `'\0'` 字符，对于空字节数组也是如此。`QByteArray()` 被认为与 `QByteArray("")` 相等。

**尽量使用 `isEmpty()` ，不推荐使用 `isNUll()   `**

## 最大的Size() 和 内存不足的状况

`QByteArray` 的最大长度不得大于 `2GB(2^31 bytes)。` 

当无法分配足够的内存时，会抛出一个异常 `std::bad_alloc `

这个异常是整个Qt容器唯一可能会抛出的异常。


## 关于区域设置的说明

**Number-String Conversions**

在进行数值数据类型与字符串之间的转换时，这些操作是在 **C语言的区域设置** （C locale）中完成的，而不是在用户的区域设置中完成。如果需要在用户的区域设置中进行 **数字与字符串的转换** ，应使用 `QString` 来执行。

**8-bit Character Comparisons**

在 `QByteArray` 中，关于大小写（大写字母和小写字母）以及字符之间的比较（即哪个字符大于或小于另一个字符）是按照 **Latin-1 编码** 的规则进行的。这会影响那些支持大小写不敏感选项、或对参数进行比较、转换为小写或大写的函数。如果两个字符串都只包含 Latin-1 字符，那么大小写不敏感的操作和比较将是准确的。受影响的函数包括：`contains()`、`indexOf()`、`lastIndexOf()`、`operator<`、`operator<=`、`operator>`、`operator>=`、`isLower()`、`isUpper()`、`toLower()` 和 `toUpper()`。

这个问题不会出现在 `QString` 中，因为 `QString` 使用 **Unicode** 来表示字符。

```cpp
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
```

```bash
QByteArray toLower(): stra?e
QString toLower(): stra?e
QByteArray comparison (A vs a): A < a
QString comparison (A vs a): A < a
```


#  Public Function

## fromRawData(const char *data, intsize)

这个方法构造的 `QByteArray` 对象是浅拷贝的，最初只是保存了一个首地址指针，所以它在做修改之前都是只读的，只是引用了原始对象的副本。

一旦视图做出修改，就会出现写时复制，直到这时才会隐式地拷贝一份数据，并在拷贝地数据上做修改。

```cpp
]#ifndef __QTBYTEARRAY_H__
#define __QTBYTEARRAY_H__
#include <iostream>
#include <QtCore/QByteArray>

void testFromRawData()
{
	const char* cstr = "QtByteArray-readOnly"; //原始数据 raw data
	int size = 10;

	QByteArray qBa = QByteArray::fromRawData(cstr, size); //只会使用前size个字符串
	QByteArray qBb = QByteArray::fromRawData(cstr, strlen(cstr));
	//fromRawData()的实例不是默认以 '\0' 结尾的， 除非 raw data 单重含有'\0'

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

#endif
```

运行结果如下，发现在修改前后，qBa Ptr 有所改变，意味着确实发生了复制，并导致地址发生了变化。

```bash
qBa data: QtByteArray-readOnly
qBa Ptr : 00007FF7CDA4ABB0
qBa size: 10
qBb data: QtByteArray-readOnly
qBa Ptr : 00007FF7CDA4ABB0
qBb size: 20
After- qBa data: QTByteArra
After- qBa Ptr : 00000265A36A1338
After- qBa size: 10
After- qBb data: QtByteArray-readOnly
After- qBa Ptr : 00007FF7CDA4ABB0
After- qBb size: 20
```

**fromRawData()的实例不是默认以 '\0' 结尾的， 除非 raw data 单重含有'\0'**

下面这个例子体现了两点：

1. `QByteArray` 可以在字符串的中间嵌入 `\0`
2. `fromRawData` 不会自动补全末尾的终结符 `\0`

```cpp
QByteArray ba1("ca\0r\0t");
  ba1.size();                     // Returns 2.
  ba1.constData();                // Returns "ca" with terminating \0.

  QByteArray ba2("ca\0r\0t", 3);
  ba2.size();                     // Returns 3.
  ba2.constData();                // Returns "ca\0" with terminating \0.

  QByteArray ba3("ca\0r\0t", 4);
  ba3.size();                     // Returns 4.
  ba3.constData();                // Returns "ca\0r" with terminating \0.

  const char cart[] = {'c', 'a', '\0', 'r', '\0', 't'};
  QByteArray ba4(QByteArray::fromRawData(cart, 6));
  ba4.size();                     // Returns 6.
  ba4.constData();                // Returns "ca\0r\0t" without terminating \0.
```



## 支持Base64编码

里面有枚举类型，每一个枚举类型，都有不同的编码和解码的规则。

```cpp
//Base64 Coding and EnCoding
void Base64() 
{
	QByteArray data = "Hello, World!";

	// 标准 Base64 编码
	QByteArray standardBase64 = data.toBase64();
	std::cout << "Standard Base64: " << standardBase64.constData() << std::endl;

	// URL 安全的 Base64 编码
	QByteArray urlBase64 = data.toBase64(QByteArray::Base64UrlEncoding);
	std::cout << "URL Base64: " << urlBase64.constData() << std::endl;

	// 省略尾部等号的 URL 安全 Base64 编码
	QByteArray urlBase64NoPadding = data.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
	std::cout << "URL Base64 (No Padding): " << urlBase64NoPadding.constData() << std::endl;

}
```

#### 1. **Base64 Alphabet Options  编码解码规则选择 `[QByteArray::Base64Options]`**

* **`QByteArray::Base64Encoding`** (值为 0， 默认值)
  * 使用标准的 Base64 字母表，称为 “base64”。
  * 标准 Base64 字母表包括字母 `A-Z`、`a-z`、`0-9`，以及符号 `+` 和 `/`。
  * 使用 `=` 作为填充字符，以确保编码后的数据长度是 4 的倍数。
* **`QByteArray::Base64UrlEncoding`** (值为 1)
  * 使用一种替代的 Base64 字母表，称为 “base64url”。
  * 这种字母表将标准 Base64 中的 `+` 和 `/` 替换为 `-` 和 `_`，使其更适合用于 URL 和文件名。
  * 例如，标准 Base64 中的 `+` 和 `/` 在 URL 中可能会被误解为特殊字符，而 `-` 和 `_` 则不会。

#### 2. **Padding Options  是否填充末尾等号**

由于Base64的编码结果总是4的倍数，当原来的编码结果不足4的倍数时，会在末尾填充等号。

* **`QByteArray::KeepTrailingEquals`** (值为 0， 默认值)
  * 保留编码数据末尾的填充等号 `=`。
  * Base64 编码要求数据长度必须是 4 的倍数。如果原始数据长度不是 3 的倍数，则会在编码后添加 `=` 作为填充字符。
* **`QByteArray::OmitTrailingEquals`** (值为 2)
  * 省略编码数据末尾的填充等号 `=`。
  * 这种选项适用于某些场景（如 URL 编码），其中填充字符 `=` 可能不被允许或需要被省略。


示例1：

```cpp
//Base64 Coding and EnCoding
void Base64() 
{
	QByteArray data = "https://www.qt.io/=%/Hello+/World!/";

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
```

```cpp
Standard Base64: aHR0cHM6Ly93d3cucXQuaW8vPSUvSGVsbG8rL1dvcmxkIS8=
Standard Base64 Encoding :https://www.qt.io/=%/Hello+/World!/

URL Base64: aHR0cHM6Ly93d3cucXQuaW8vPSUvSGVsbG8rL1dvcmxkIS8=
urlBase64 Encoding :https://www.qt.io/=%/Hello+/World!/

URL Base64 (No Padding): aHR0cHM6Ly93d3cucXQuaW8vPSUvSGVsbG8rL1dvcmxkIS8=
Decoded URL Base64 (With Padding): https://www.qt.io/=%/Hello+/World!/

URL Base64 (No Padding): aHR0cHM6Ly93d3cucXQuaW8vPSUvSGVsbG8rL1dvcmxkIS8
Decoded URL Base64 (With Padding): https://www.qt.io/=%/Hello+/World!/
```

会发现输出的结果完全一样，为什么，难道两种编码相同吗？

那究竟有什么区别，所谓的Url编码，并不是规避源字符串里面的 `+` 和 `/` ，而是在解码的时候规避。

示例2：其余和示例1一样，源字符串加上 `?` ，这个字符的编码含有 `_` 

```cpp
QByteArray data = "https://www.qt.io?/=%/Hello+?/World!?/";
```

输出结果：

```cpp
Standard Base64: aHR0cHM6Ly93d3cucXQuaW8/Lz0lL0hlbGxvKz8vV29ybGQhPy8=     //里面有 ‘/’
Standard Base64 Encoding :https://www.qt.io?/=%/Hello+?/World!?/

URL Base64: aHR0cHM6Ly93d3cucXQuaW8_Lz0lL0hlbGxvKz8vV29ybGQhPy8=          //所有  ‘/’ 换成了 ‘_’
urlBase64 Encoding :https://www.qt.io?/=%/Hello+?/World!?/

URL Base64 (No Padding): aHR0cHM6Ly93d3cucXQuaW8_Lz0lL0hlbGxvKz8vV29ybGQhPy8=
Decoded URL Base64 (With Padding): https://www.qt.io?/=%/Hello+?/World!?/

URL Base64 (No Padding): aHR0cHM6Ly93d3cucXQuaW8_Lz0lL0hlbGxvKz8vV29ybGQhPy8
Decoded URL Base64 (With Padding): https://www.qt.io?/=%/Hello+?/World!?/
```
