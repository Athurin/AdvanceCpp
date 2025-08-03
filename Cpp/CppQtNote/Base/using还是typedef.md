在现代 C++（C++11 及以后）中，**优先使用 `using`**。

---

### ✅ 为什么？

| 对比项 | `using` | `typedef` |
|--------|---------|-----------|
| **语法清晰** | `using value_type = T;` | `typedef T value_type;` |
| **支持模板别名** | ✅ `template<class U> using ptr = U*;` | ❌ 无法实现 |
| **可读性** | 左值是别名，右边是类型，直观 | 顺序相反，易读性差 |
| **现代风格** | C++11 引入，推荐 | 仅用于兼容旧代码 |

---

### ✅ 示例对比

```cpp
// ✅ 推荐：using
using value_type = T;

// ❌ 旧式：typedef
typedef T value_type;
```

---

### ✅ 结论

> **除非需要兼容 C++03，否则一律用 `using`。**  
`using` 语法更直观，支持模板别名，是现代 C++ 的推荐写法。