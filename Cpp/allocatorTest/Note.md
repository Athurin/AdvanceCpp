```cpp
#include <iostream>
#include <memory>


int main()
{
    std::allocator<int> alloc;
    int* p = alloc.allocate(5); // 分配 5 个 int 的内存

    for (int i = 0; i < 5; ++i) {
        std::construct_at(p + i, 0); // C++20 推荐方式
    }

    for (int i = 0; i < 5; ++i) {
        std::cout << p[i] << " "; // 输出: 0 0 0 0 0
    }

    for (int i = 0; i < 5; ++i) {
        std::destroy_at(p + i); // 析构对象
    }
    alloc.deallocate(p, 5); // 释放内存

	return 0;
}
```

上述例子使用了 `C++20` 的alloctor.

在 `cmake` 构建中使用:

```cpp
set(CMAKE_CXX_STANDARD 20)
```

如果顶层目录设置了C++标准是C++11，子项目依旧可以设置，并且只在当前子项目起作用。

| 步骤                      | 使用的函数                      | 作用                                                  |
| ------------------------- | ------------------------------- | ----------------------------------------------------- |
| **1. 分配原始内存** | `alloc.allocate(5)`           | 分配未初始化的内存（只是“空地”）                    |
| **2. 构造对象**     | `std::construct_at(p + i, 0)` | 在已分配的内存上构造 `int` 对象，初始化为 `0`     |
| **3. 使用对象**     | `std::cout << p[i]`           | 正常访问                                              |
| **4. 析构对象**     | `std::destroy_at(p + i)`      | 调用析构函数（对 `int` 是空操作，但对类类型很重要） |
| **5. 释放内存**     | `alloc.deallocate(p, 5)`      | 归还内存给分配器                                      |

`std::allocate ` 负责分配和管理内存

`std::construct_at` **不负责内存分配** ，它只负责“在已有内存上调用构造函数”。

如下的例子阐明了内存分配器类型的重要性：

```cpp
int main()
{
    std::allocator<std::string> alloc;
    std::string* p = alloc.allocate(5); // 分配 5 个 string 的内存

    for (int i = 0; i < 5; ++i)
        std::construct_at(p + i, std::string("zzz")); // 构造string的实例
  

    for (int i = 0; i < 5; ++i) 
        std::cout << p[i] << " "; // 输出: zzz zzz zzz zzz zzz
  

    for (int i = 0; i < 5; ++i) 
        std::destroy_at(p + i); // 析构string对象
  
    alloc.deallocate(p, 5); // 释放内存

	return 0;
}

```

实现一个简单的 std::allocator

头文件定义如下：

```cpp
#ifndef __ATHALLOCATOR_H__
#define __ATHALLOCATOR_H__

#include <cstddef>

template<typename T>
class AthAllocator
{
public:
	AthAllocator();
	~AthAllocator();

	using value_type		=   T;
	using pointer			=	value_type*;
	using reference			=	value_type&;
	using const_pointer		=	const value_type*;
	using const_reference	=	const value_type&;
	using size_type			=	size_t;
	using difference_type	=	ptrdiff_t;

	// rebind...
	//template <typename _other> struct rebind { typedef CHxAllocator<_other> other; };


	pointer allocate(size_type n);
	void deallocate(pointer p, size_type n = 0) noexcept;
	size_type max_size() const noexcept;

	void construct(pointer p, const_reference val);
	void destroy(pointer p);

	// 比较运算符
	friend bool operator==(const AthAllocator&, const AthAllocator&) noexcept
	{
		return true;
	}
	friend bool operator!=(const AthAllocator& a, const AthAllocator& b) noexcept
	{
		//return !(a == b);错误
		return false;
	}

};

#endif // !__ATHALLOCATOR_H__

```

值得注意的是，相比于C++03，C++20的标准已经舍弃原来的一些成员方法。

相比于博客：[C++ allocator - wpcockroach - 博客园](https://www.cnblogs.com/wpcockroach/archive/2012/05/10/2493564.html)

区别在于：

• 不实现 `construct/destroy`，标准会帮你做。
• 不实现双参 `allocate` 也行，容器只调用单参版。
• 不实现 `rebind`（C++17 起已废弃），因为 `AthAllocator<T>` 本身就是模板。

### 构造析构

```cpp
template<typename T>
AthAllocator<T>::AthAllocator()
{

}

template<typename T>
AthAllocator<T>::~AthAllocator()
{

}
```

### allocate

```cpp
template<typename T>
typename AthAllocator<T>::pointer
AthAllocator<T>::allocate(size_type n)
{
    if (n > max_size()) throw std::bad_alloc{};
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
}
```

1. **为什么使用 `typename` ?**

    C++ 标准规定：
	当编译器在模板定义阶段看到`AthAllocator<T>::pointer` 这样的名字时，如果前面没有 `typename`，它默认认为这是一个 **值或数据成员** ，而不是 **类型** 。
	因此，写成

```cpp
AthAllocator<T>::pointer
```

    会被编译器理解为“访问某个叫`pointer` 的静态数据成员”——显然不对，于是报错。
	在 `typename AthAllocator<T>::pointer` 中，`typename` 明确告诉编译器：“ **后面这个标识符是一个类型** ”，从而正确解析。


2. **解析函数体**

   ```cpp
   return static_cast<pointer>(::operator new(n * sizeof(value_type)));
   ```

    这行代码的含义是：

1. `::operator new(n * sizeof(value_type))`调用 **全局**（前面的 `::` 表示全局作用域）的 `operator new` 函数，申请一块足够大的 **原始内存**（raw memory），大小为 `n * sizeof(value_type)` 字节。
2. `static_cast<pointer>(...)`
   把 `void*` 类型的原始内存指针 **强制转换** 成 `AthAllocator<T>::pointer` 类型，也就是 `T*`。
   这样返回值就与 `allocate` 的声明（返回 `pointer` 即 `T*`）完全匹配。
   理解为：“向系统申请一块能容纳 n 个 T 的原始内存，并把它当作 T 的指针返回给调用者。”

### deallocate

```cpp
template<typename T>
void AthAllocator<T>::deallocate(pointer p, size_type) noexcept
{
    ::operator delete(p);
}
```


### max_size

```cpp
template<typename T>
typename AthAllocator<T>::size_type 
AthAllocator<T>::max_size() const noexcept
{
    return SIZE_MAX / sizeof(value_type);
}
```

计算  **这个 allocator 一次最多能分配多少个 T 元素** 。

公式拆开来看：

`max_size() = SIZE_MAX / sizeof(value_type)`

* `SIZE_MAX` 是 `<cstddef>`（或 `<climits>`）里定义的 `size_t` 能表示的最大值，通常是 2⁶⁴-1 或 2³²-1。
* `sizeof(value_type)` 即 `sizeof(T)`，是每个元素占用的字节数。
* 两者相除，就得到  **在不溢出 `size_t` 的前提下，最多可以一次性申请多少个 T 对象** 。

### 内存再构造和析构

```cpp
template<typename T>
void AthAllocator<T>::construct(AthAllocator<T>::pointer p, AthAllocator<T>::const_reference val)
{
    :: new ((void *)p) T(val); //把 p 强制转换为 void* 类型
}

template<typename T>
void AthAllocator<T>::destroy(AthAllocator<T>::pointer p)
{
    p->~T();
}
```

### == 和 !=

```cpp
	// 比较运算符
	friend bool operator==(const AthAllocator&, const AthAllocator&) noexcept
	{
		return true;
	}
	friend bool operator!=(const AthAllocator& a, const AthAllocator& b) noexcept
	{
		//return !(a == b);错误
		return false;
	}、、
```



1. **无状态 allocator 的定义**
   无状态 allocator 没有内部状态，因此任何两个实例都是等价的。这意味着：
   * `operator==` 应该总是返回 `true`。
   * `operator!=` 应该总是返回 `false`。
2. **标准库的要求**
   C++17 及以后的标准库容器要求 allocator 的比较运算符必须符合无状态的定义。如果 `operator!=` 依赖于 `operator==`，可能会导致编译器警告或错误。


## 关于编译时的链接错误

最开始的模板类使用.h .cpp的分离模式，但是这会导致链接问题。

.h文件

```cpp
#ifndef __ATHALLOCATOR_H__
#define __ATHALLOCATOR_H__

#include <cstddef>
#include <limits.h>
#include <stdexcept>
#include <string>
#include <iostream>

template<typename T>
class AthAllocator
{
public:
	AthAllocator();
	~AthAllocator();

	using value_type		=   T;
	using pointer			=	value_type*;
	using reference			=	value_type&;
	using const_pointer		=	const value_type*;
	using const_reference	=	const value_type&;
	using size_type			=	size_t;
	using difference_type	=	ptrdiff_t;

	// rebind...
	//template <typename _other> struct rebind { typedef CHxAllocator<_other> other; };


	pointer allocate(size_type n);
	void deallocate(pointer p, size_type n = 0) noexcept;
	size_type max_size() const noexcept;

	void construct(pointer p, const_reference val);
	void destroy(pointer p);

	// 比较运算符
	friend bool operator==(const AthAllocator&, const AthAllocator&) noexcept
	{
		return true;
	}
	friend bool operator!=(const AthAllocator& a, const AthAllocator& b) noexcept
	{
		//return !(a == b);错误
		return false;
	}

};

#endif // __ATHALLOCATOR_H__

```

.cpp文件

```cpp
#include "Allocator.h"

template<typename T>
AthAllocator<T>::AthAllocator()
{

}

template<typename T>
AthAllocator<T>::~AthAllocator()
{

}
 

template<typename T>
typename AthAllocator<T>::pointer
AthAllocator<T>::allocate(size_type n)
{
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
}


template<typename T>
void AthAllocator<T>::deallocate(pointer p, size_type n) noexcept
{
    ::operator delete(p);
}

template<typename T>
typename AthAllocator<T>::size_type 
AthAllocator<T>::max_size() const noexcept
{
    return SIZE_MAX / sizeof(value_type);
}


template<typename T>
void AthAllocator<T>::construct(AthAllocator<T>::pointer p, AthAllocator<T>::const_reference val)
{
    :: new ((void *)p) T(val); //把 p 强制转换为 void* 类型
}

template<typename T>
void AthAllocator<T>::destroy(AthAllocator<T>::pointer p)
{
    p->~T();
}
```

以上模式会导致链接错误。

提供以下几种方法解决：

1. 直接在模板类内定义，简单。
2. 在.cpp文件下显式实例化该模板。但这种方法需要在源文件中显式实例化所有需要的模板类型，增加了编译的复杂性，也降低了代码的灵活性。
   在.cpp文件末尾加上所有需要的模板实例化类型：

   ```cpp
   // 显式实例化
   template class AthAllocator<std::string>;
   ```
3. 使用.tcc文件，将定义部分放在.tcc文件中。

   在文件.h的末尾加上

```cpp
#include "Allocator.tcc"
```

Allocator.tcc的内容如下：

```cpp

template<typename T>
AthAllocator<T>::AthAllocator()
{

}

template<typename T>
AthAllocator<T>::~AthAllocator()
{

}
 

template<typename T>
typename AthAllocator<T>::pointer
AthAllocator<T>::allocate(size_type n)
{
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
}


template<typename T>
void AthAllocator<T>::deallocate(pointer p, size_type n) noexcept
{
    ::operator delete(p);
}

template<typename T>
typename AthAllocator<T>::size_type 
AthAllocator<T>::max_size() const noexcept
{
    return SIZE_MAX / sizeof(value_type);
}


template<typename T>
void AthAllocator<T>::construct(AthAllocator<T>::pointer p, AthAllocator<T>::const_reference val)
{
    :: new ((void *)p) T(val); //把 p 强制转换为 void* 类型
}

template<typename T>
void AthAllocator<T>::destroy(AthAllocator<T>::pointer p)
{
    p->~T();
}
```

.tcc用于分离模板的定义部分，但是切记放在头文件的最下面，这样的顺序便是 “声明 + 定义”，不会编译出错。



**测试用例如下：**

```cpp

#include "Allocator.h"

//Test
int main()
{
	AthAllocator<std::string> all;

    std::string* p = all.allocate(5); // 分配 5 个 string 的内存

    for (int i = 0; i < 5; ++i)
        all.construct(p + i, std::string("zzz")); // 构造string的实例


    for (int i = 0; i < 5; ++i)
        std::cout << p[i] << " "; // 输出: zzz zzz zzz zzz zzz


    for (int i = 0; i < 5; ++i)
        all.destroy(p + i); // 析构string对象

    all.deallocate(p, 5); // 释放内存


	return 0;
}
```











end
