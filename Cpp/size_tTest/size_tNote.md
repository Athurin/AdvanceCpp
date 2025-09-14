# size_t

## main.cpp:

注意无符号整数的循环条件不能是：i>=0；

```cpp
#include <array>
#include <cstddef>
#include <iostream>

int main()
{
    std::array<std::size_t, 10> a;

    // Example with C++23 std::size_t literal
    //for (auto i = 0uz; i != a.size(); ++i)
    //    std::cout << (a[i] = i) << ' ';
    //std::cout << '\n';

    for(size_t i = 0; i != a.size(); i++)
        std::cout << (a[i] = i) << ' ';
    std::cout << '\n';

    // Example of decrementing loop
    for (std::size_t i = a.size(); i--;)//i == 0时 false跳出循环
        std::cout << a[i] << ' ';
    std::cout << '\n';

    // Note the naive decrementing loop:
    //  for (std::size_t i = a.size() - 1; i >= 0; --i) ... //i>=0恒为真
    // is an infinite loop, because unsigned numbers are always non-negative
}
```

这段代码利用了 `size_t`（无符号类型）的**下溢特性**来优雅地实现**逆向遍历**而不写出无限循环。我们来逐步解释它的原理和为什么“朴素递减循环”会出问题。

---

### ✅ 正确写法解析

```cpp
for (std::size_t i = a.size(); i--;)
    std::cout << a[i] << ' ';
```

#### 🔍 执行流程：

- 初始：`i = a.size()`（比如是 5）
- 每次循环前：**先判断 `i--` 的值**
  - 因为是**后缀递减**，表达式值**是原来的 `i`**
  - 当 `i == 0` 时，`i--` 表达式值是 `0`，判断为 **false**，**循环结束**
  - 然后 `i` 变成 `size_t(-1)`，即最大值（但不会再被使用）

✅ **优点**：

- 没有负数比较
- 没有下溢风险
- 简洁、安全、常用于逆向遍历

---

### ❌ 错误写法解析（无限循环）

```cpp
for (std::size_t i = a.size() - 1; i >= 0; --i) {
    std::cout << a[i] << ' ';
}
```

#### 🔥 问题：

- `i` 是 `std::size_t`，**无符号类型**
- 条件 `i >= 0` 对无符号数**永远为真**
- 当 `i == 0` 时，`--i` 会让 `i` 下溢为 `size_t(-1)`（即最大值）
- 然后继续循环，**永远不会退出** → **无限循环**

---

### ✅ 替代正确写法（显式判断）

如果你不想用 `i--` 的技巧，也可以这样写：

```cpp
for (std::size_t i = a.size(); i-- > 0;) {
    std::cout << a[i] << ' ';
}
```

或者使用 `int` 索引（但要小心类型转换和负数）：

```cpp
for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
    std::cout << a[i] << ' ';
}
```

---

### ✅ 总结一句话

> **无符号数递减时，`i >= 0` 恒为真**，必须用 `i--` 或 `i-- > 0` 等方式**避免负数判断**，否则就是**无限循环**。
>
