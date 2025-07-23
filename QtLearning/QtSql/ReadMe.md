# QSqlDatabase

提供了通过连接的方式去访问数据库的接口，一个 `QSqlDatabase` 的实例就象征着一个数据库的连接。

> 访问数据库的连接在查询还在进行的时候不会释放，就有可能导致多个查询访问一个数据的时候出现连接占用的问题，需要重新定义一个 `QSqlDatabase` 的实例。

Qt 框架中的 `lastError()`函数

* **功能** ：在 Qt 的数据库模块中，`lastError()`函数用于获取最后一次数据库操作的错误信息，返回一个 `QSqlError`对象。
* **使用方式** ：当进行数据库操作时，若操作失败，可调用与数据库连接相关的对象的 `lastError()`函数来获取错误信息，以便进行相应的错误处理和调试。

```cpp
// 获取数据库连接错误信息
QSqlError err = db.lastError();
QMessageBox::critical(this, "Database Error", err.text());

//可以直接使用text()函数来访问错误的文本信息
QSqlError err = db.lastError().text()
```

*Qt* 官方推荐 `SQLite` 数据库，因为可以全平台适应，测试覆盖率表现极好。

> [SQLite](qtsql-attribution-sqlite.html#sqlite) is the in-process database system with the best test coverage and support on all platforms.

**in-process : 表示“进程内部”。SQLite 是一个嵌入式数据库管理系统，它的 “in - process” 特性意味着数据库引擎与应用程序在同一个进程中运行。这使得应用程序可以直接与数据库引擎进行交互，无需通过网络通信或其他进程间通信方式。**

**因为[SQLite](qtsql-attribution-sqlite.html#sqlite)是 in - process 的，所以不支持多进程同时写入同一个数据库文件（虽然它可以处理多线程访问的情况，但有一些限制）。如果多个进程需要同时访问和修改数据库，可能需要采用其他机制来协调访问，或者使用其他类型的数据库管理系统，如客户端 - 服务器模式的数据库（如 MySQL、PostgreSQL）。**

```cpp
bool 
connectCreate
(const QString& conName, const QString& hostName, const QString& DBName, const QString& User, const QString& Password )
{
	QSqlDatabase db = QSqlDatabase::addDatabase(conName); //static  "连接名
	db.setHostName(hostName);
	db.setDatabaseName(DBName);
	db.setUserName(User);
	db.setPassword(Password);

	return db.open();

}
```

`addDatabase` 是 `static` 的。

一个连接靠名字标识而不是靠他所连接的数据库。一个数据库可以同时有多个连接；但是一个连接一次只能做一次查询。

**默认连接：是无名的。在调用 `addDatabase` 时不要传递名字，就能创建默认连接。**

**默认连接创建完成之后就能够在任意静态成员函数中，哪怕不指定连接名字也能使用。**

```cpp
//默认连接 
QSqlDatabase db = QSqlDatabase::database();
```

注意：***在没有open()之前，连接是不起效的。***

`QSqlDatabase` 是一个 `值类` value class ：

通过一个 `QSqlDatabase`实例对数据库连接所做的更改，将影响代表同一连接的其他 `QSqlDatabase`实例。如果需要基于现有连接创建一个独立的数据库连接，请使用 `cloneDatabase()`。

（不同的实例却可以代表同一个连接，因为靠连接的名字标识）

注意：**最好不要在类中维护一个 `QSqlDatabase ` 的实例的副本，这会阻止连接正常关闭。如果仍要维护一个 `QSqlDataBase`的成员变量，应该在这个类对象析构之前，销毁 `QSqlDataBase`的成员变量。否则，会引发未定义的行为。**

如果建立了多个数据库的连接，每一个都使用单独的名字。

> Use [database](qsqldatabase.html#database)() with a connection name to get that connection.
>
> Use [removeDatabase](qsqldatabase.html#removeDatabase)() with a connection name to remove a connection.

如果某个实例在别的地方被引用了，当试图remove连接的时候，会出现警告。

可以使用 `contains() ` 查看连接名称是否出现在已有的连接名称当中。

## QSqlDatabase()

创建出来的实例是无效的，空的。不会初始化任何实际的数据库连接信息。

## 拷贝构造 QSqlDatabase(const [QSqlDatabase](qsqldatabase.html#QSqlDatabase) &other)

create a copy

## 重载 =

assign

## [static] addDatabase

**线程安全的**

在连接表中添加名称为 `connectionName` 的新的连接，如果连接名已经存在，就会把同名旧的连接移除，然后添加新的连接，最后返回新的连接的实例。

如果 `type` 没有正确加载或者不可用，`isValid()` 函数可以检查，并且返回 `false` .

1. 如果连接名没有被指定，那么该连接就会成为这个应用的默认连接，后续所有 `database()` 的调用都会默认使用这个默认的无名连接。
2. 如果指定了连接名，使用 `database(connectionName) ` 去检索（获取）连接。

同名的连接，新的会取代旧的。

在 Qt 中，当调用 `QSqlDatabase::addDatabase()` 函数来添加数据库连接时，如果没有提供连接名（`connectionName`），则该连接会被视为默认连接。**如果多次调用 `addDatabase()` 函数而没有指定连接名，那么每次都只会更新默认连接，也就是用新的连接配置替换默认连接。**

在使用连接之前，要记得初始化。

> call some or all of
>
> [setDatabaseName](qsqldatabase.html#setDatabaseName)(),  //2
>
> [setUserName](qsqldatabase.html#setUserName)(),  //3
>
> [setPassword](qsqldatabase.html#setPassword)(),  //4
>
> [setHostName](qsqldatabase.html#setHostName)(),   //1
>
> [setPort](qsqldatabase.html#setPort)(),
>
> [setConnectOptions](qsqldatabase.html#setConnectOptions)(),
>
> and, finally,
>
> [open](qsqldatabase.html#open)().  //5

egl:
