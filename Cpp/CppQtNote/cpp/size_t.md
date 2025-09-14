[std::size_t - cppreference.cn - C++参考手册](https://cppreference.cn/w/cpp/types/size_t)

`std::size_t` 是下列运算符结果的无符号整数类型：
```
sizeof()
alignof()
```
如果程序试图形成一个过大的类型（即，其[对象表示](https://cppreference.cn/w/cpp/language/object#Object_representation_and_value_representation "cpp/language/object")中的字节数超过 `std::size_t` 中可表示的最大值），则程序格式错误。

`std::size_t` 的位宽不小于 16。

`std::size_t` 可以存储任何类型（包括数组）理论上可能的最大对象大小。在许多平台上（分段地址系统除外），`std::size_t` 可以安全地存储任何非成员指针的值，在这种情况下，它与 [std::uintptr_t](https://cppreference.cn/w/cpp/types/integer "cpp/types/integer") 是同义词。

`std::size_t` 通常用于数组索引和循环计数。使用其他类型（例如 unsigned int）进行数组索引的程序可能会在例如 64 位系统上失败，如果索引超过 [UINT_MAX](https://cppreference.cn/w/cpp/types/climits "cpp/types/climits") 或依赖于 32 位模运算。

在索引 C++ 容器（例如 [std::string](https://cppreference.cn/w/cpp/string/basic_string "cpp/string/basic string")、[std::vector](https://cppreference.cn/w/cpp/container/vector "cpp/container/vector") 等）时，适当的类型是这些容器提供的嵌套类型 `size_type`。它通常被定义为 `std::size_t` 的同义词。

`std::size_t` 的声明是否在任何其他标准库头文件中可用是未指定的。即使标准要求使用 `std::size_t`，实现也可以避免引入此名称。

`std::size_t` 的[整数字面量后缀](https://cppreference.cn/w/cpp/language/integer_literal "cpp/language/integer literal")是 `z` 或 `Z` 与 `u` 或 `U` 的任意组合（即 `zu`、`zU`、`Zu`、`ZU`、`uz`、`uZ`、`Uz` 或 `UZ`）。（C++23起）

> [!NOTE]
> 还是无法理解size_t，作为一个无符号整型为什么不直接使用usigned int

一句话先给答案：  
`size_t` 并不是“又一个 `unsigned int` 别名”，而是**平台无关地保证“足够装得下任何对象大小”** 的最小无符号类型。  
用 `unsigned int` 会踩三颗雷：范围可能不够、移植性丢失、接口契约被破坏。下面分点拆开。

------------------------------------------------
1. 范围不够——32 位 vs 64 位（表示地址的时候）

- 32 位系统  
  `unsigned int` 通常 32 位 → 最大 4 GB。  
  但**单个对象**最大就可能到 4 GB（`SIZE_MAX` ≈ 4 GB），刚好满格；一旦代码里出现大于 4 GB 的数组/容器，就会溢出。

- 64 位系统  
  `unsigned int` 仍旧 32 位（Windows/Linux 都保持 32 bit），而**地址空间**、**文件长度**、**容器大小**都可以远超过 4 GB。  
  `size_t` 在 64 位平台被定义为 64 位无符号整数 → 上限 16 EB（16 × 10^18），足够覆盖理论最大对象。

------------------------------------------------
2. 移植性——同样的源码，不同行为

```cpp
// 错误示范：假设用 unsigned int 存容器大小
std::vector<double> v(1ULL << 33);   // 8 × 2^33 ≈ 68 GB
unsigned int n = v.size();           // 64 位机上高位被截断！
```
把 `unsigned int` 换成 `size_t` 立即安全，因为 `size_t` 会随平台自动“长大”。

------------------------------------------------
3. 标准库契约——整套 API 都以 `size_t` 为度量单位

- 每个容器 `size()/length()` 返回 `size_t`  
- `sizeof` 运算符结果类型是 `size_t`  
- `malloc`/`memcpy`/`strlen`/`std::string`/`std::span`……  
  凡涉及“字节数/元素个数”的参数全是 `size_t`。  

如果你硬用 `unsigned int`：

```cpp
void* p = malloc(UINT_MAX + 1ULL); // 64 位机要 4 GB+1 字节
// 传参时被隐式截断，实际只申请 0 字节！
```

------------------------------------------------
4. 历史包袱——C 从一开始就分开

C 标准（C89 起）把 `size_t` 规定成**“足够表示地址空间”**的无符号类型；  
C++ 直接继承并放进命名空间 `std::`。  
`unsigned int` 的宽度由  “整数模型”**（ILP32/LP64/LLP64）决定，与地址模型无关，因此不能承担“大小/长度”角色。

------------------------------------------------
5. 一句话记忆

`unsigned int` 是**固定 32 位**的算术整数；  
`size_t` 是**“多大都能装”的尺码单位**，随平台自动伸缩。  
写与“大小、长度、内存”相关的代码时，永远用 `size_t`——这是标准库、操作系统、编译器三方早已写好的契约。


> [!NOTE] Title
> size_t 是用来表示地址空间的无符号整型，随地址模型变化。
> 
