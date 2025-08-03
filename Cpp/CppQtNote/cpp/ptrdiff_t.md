
> [!NOTE]
> Result of pointer subtraction
> 
> Alias of one of the [fundamental](https://cplusplus.com/is_fundamental) signed integer types.  
>   
> It is a type able to represent the result of any valid pointer subtraction operation.  
>   
> A pointer subtraction is only guaranteed to have a valid defined value for pointers to elements of the same array (or for the element just past the last in the array).


`ptrdiff_t` 是 C/C++ 标准定义的**有符号整数类型**，专门用来表示**两个指针相减的结果**（即“距离”或“差值”）。

---

### ✅ 基本要点

| 名称 | 说明 |
|------|------|
| 头文件 | `<cstddef>`（C++） / `<stddef.h>`（C） |
| 类型 | **有符号整数**（通常是 `long` 或 `long long`） |
| 用途 | 保存两个同类型指针相减的结果 |

---

### ✅ 示例代码

```cpp
#include <iostream>
#include <cstddef>

int main() {
    int arr[10];
    int* p1 = &arr[2];
    int* p2 = &arr[7];

    std::ptrdiff_t dist = p2 - p1;  // 合法：差值 = 5
    std::cout << "distance = " << dist << '\n';
}
```

输出：
```
distance = 5
```

---

### ⚠️ 注意事项

1. **只能用于指向同一数组区间的指针**，否则行为未定义。
2. **有符号**，因此可以表示负距离。
3. **在 64 位平台上**通常 64 位，但标准只保证“足以表示任何对象大小”。

---

### ✅ 与 `size_t` 的区别

| 类型 | 符号 | 常见用途 |
|------|------|----------|
| `size_t` | **无符号** | 数组索引、对象大小、容器 `size()` |
| `ptrdiff_t` | **有符号** | 指针差值、迭代器距离 |

---

### ✅ 一句话总结

> `ptrdiff_t` 是“指针差值的官方类型”，用它来表示两个指针之间的距离最符合标准、最安全。