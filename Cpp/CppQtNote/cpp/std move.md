一句话记忆  
**`std::move` 并不“移动”任何东西，它只是把实参强制转换成右值引用（`T&&`），从而**启用**重载决议中的“移动”版本。**

---

### 1. 本质签名（C++11 起）

```cpp
template<class T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}
```

- 只做 **强制类型转换**（`static_cast`）。  
- 不生成代码，不复制、不移动、不分配，**零成本**（编译期）。  

---

### 2. 工作流程

```cpp
std::string a = "hello";
std::string b = std::move(a);   // 1. 转换  2. 选择移动构造
```

1. `std::move(a)` → `static_cast<std::string&&>(a)`  
2. 重载决议：`std::string(std::string&&)`（移动构造）被选中。  
3. 移动构造完成后，`a` 处于 **有效但未指定状态**（可析构、可赋值，但值不确定）。

---

### 3. 常见误区

| 误区 | 正解 |
|---|---|
| `std::move` 会调用移动构造 | **不会**，它只是把值变成右值引用；真正的移动由接收方完成。 |
| 用 `std::move` 后原对象立刻销毁 | **不会**，生命周期完全由作用域控制。 |
| `const T` 可以 `std::move` 得到移动 | **失败**，`const T&&` 会绑定到拷贝而非移动。 |

---

### 4. 何时必须用 `std::move`

- 把 **左值** 显式标记为“可移动”：  
  ```cpp
  std::vector<int> tmp = {1,2,3};
  v.insert(v.end(), std::move(tmp));   // 触发 vector 的移动构造
  ```
- 与 **完美转发** 区分：  
  ```cpp
  template<class T>
  void wrapper(T&& arg) {
      foo(std::forward<T>(arg));   // 保持值类别
      foo(std::move(arg));         // 强制移动
  }
  ```

---

### 5. 一句话总结

> **`std::move` 只是把左值“贴标签”为右值引用，真正的资源搬家由对应类型的移动构造/移动赋值完成。**